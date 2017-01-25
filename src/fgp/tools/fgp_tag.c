#include "fgp_tag.h"

int fgp_tag_new(fgp_tag *s)
{	
	s->degree = 1;
	g1_new(&(s->T));
	g1_set_infty(&(s->T));
	g1_new(&(s->X));
	g1_set_infty(&(s->X));
	g2_new(&(s->U));
	g2_set_infty(&(s->U));
	g2_new(&(s->Y));
	g2_set_infty(&(s->Y));

	gt_new(s->X2);
	gt_new(s->T2);
	gt_new(s->lambda);
	return 0;
}

int fgp_tag_free(fgp_tag *s)
{
	g1_free(&(s->T));
	g1_free(&(s->X));
	g2_free(&(s->U));
	g2_free(&(s->Y));

	gt_free(s->T2);
	gt_free(s->X2);	
	gt_free(s->lambda);

	free(s);
	return 0;
}

int fgp_tag_clean(fgp_tag *s)
{
	g1_free(&(s->T));
	g1_free(&(s->X));
	g2_free(&(s->U));
	g2_free(&(s->Y));

	gt_free(s->T2);
	gt_free(s->X2);	
	gt_free(s->lambda);

	return 0;
}

int fgp_tag_copy(fgp_tag * s_out, fgp_tag * s_in)
{
	s_out->degree = s_in->degree;
	g1_copy(&(s_out->T), &(s_in->T));
	g1_copy(&(s_out->X), &(s_in->X));
	g2_copy(&(s_out->U), &(s_in->U));
	g2_copy(&(s_out->Y), &(s_in->Y));
		
	gt_copy(s_out->T2, s_in->T2);
	gt_copy(s_out->X2, s_in->X2);
	gt_copy(s_out->lambda, s_in->lambda);

	return 0;
}

int fgp_tag_const_mult(fgp_tag *s_out, fgp_tag *s_in, bn_st * c)
{
	switch (s_in->degree)
	{
		case 1:
			// Set the output degree to 1 also
			s_out->degree = 1;
			// Compute the multiplications
			g1_mul(&(s_out->T), &(s_in->T), c);
			g2_mul(&(s_out->U), &(s_in->U), c);
			g1_mul(&(s_out->X), &(s_in->X), c);
			g2_mul(&(s_out->Y), &(s_in->Y), c);
			break;
		case 2:
			// Set the output degree to 2 also
			s_out->degree = 2;
			// Compute the multipliations
			gt_exp(s_out->T2, s_in->T2, c);
			gt_exp(s_out->X2, s_in->X2, c);
			gt_exp(s_out->lambda, s_in->lambda, c);
			break;
		default:
			error_hdl(-1,"Degree of a tag has to be either 1 or 2");
			return 1;
	};
	return 0;
}

int fgp_tag_const_add(fgp_tag *s_out, fgp_tag *s_in, bn_st * c)
{
	// Initialize the group generators
	g1_t g;
	g2_t h;
	g1_new(g);
	g1_get_gen(g);
	g2_new(h);
	g2_get_gen(h);

	gt_t gen_t;
	gt_new(gen_t);
	pc_map(gen_t, g, h);

	// Copy the input into the output
	fgp_tag_copy(s_out, s_in);
	switch (s_in->degree)
	{
		// In this case:
		//		T <- T·g^c
		// 		U <- U·h^c
		case 1:
			g1_mul(g, g, c);
			g2_mul(h, h, c);
			g1_add_norm(&(s_out->T), &(s_out->T), g);
			g2_add_norm(&(s_out->U), &(s_out->U), h);
			break;
		// In this case:
		//		T <- T·e(g,h)^c
		case 2:
			gt_exp(gen_t, gen_t, c);
			gt_mul(s_out->T2, s_out->T2, gen_t);
			break;
		default:
			error_hdl(-1,"Degree of a tag has to be either 1 or 2");
			return 1;
	};
	g1_free(g);
	g2_free(h);
	gt_free(gen_t);
	return 0;
}

int fgp_tag_const_sub(fgp_tag *s_out, fgp_tag *s_in, bn_st * c)
{
	bn_t mod;
	bn_new_size(mod, RELIC_DIGS);
	g1_get_ord(mod);

	bn_t opposite;
	bn_new_size(opposite, RELIC_DIGS);
	bn_mod(c, c, mod);
	bn_sub(opposite, mod, c);

	fgp_tag_const_add(s_out, s_in, opposite);

	return 0;
}

int fgp_tag_mul(fgp_tag * s, fgp_tag * s1, fgp_tag * s2)
{
	if (s1->degree + s2->degree != 2)
	{
		error_hdl(-1,"Both tags have to be of degree 1 to be multiplied together");
		return 1;
	}

	// T = e(T1,U2)
	pc_map(s->T2, &(s1->T), &(s2->U));
	// lambda = e(X1,Y2)
	pc_map(s->lambda, &(s1->X), &(s2->Y));

	// X = e(X1,U2)·e(X2,U1)
	gt_t temp1, temp2;
	gt_new(temp1);
	gt_new(temp2);

	pc_map(temp1, &(s1->X), &(s2->U));
	pc_map(temp2, &(s2->X), &(s1->U));
	gt_mul(s->X2, temp1, temp2);

	// Set output degree to 2
	s->degree = 2;

	gt_free(temp1);
	gt_free(temp2);
	return 0;
}

int fgp_tag_add(fgp_tag * s, fgp_tag * s1, fgp_tag * s2)
{
	g2_t h;
	g2_new(h);
	g2_get_gen(h);

	int degree_sum = 2*(s1->degree) + s2->degree;
	switch (degree_sum)
	{
		case 3:		// This means they are both of degree 1
			g1_add_norm(&(s->T),&(s1->T),&(s2->T));
			g2_add_norm(&(s->U),&(s1->U),&(s2->U));
			g1_add_norm(&(s->X),&(s1->X),&(s2->X));
			g2_add_norm(&(s->Y),&(s1->Y),&(s2->Y));
			s->degree = 1;
			break;
		case 4:		// s1 is of degree 1 and s2 of degree 2
			// Create a degree 2 fgp_tag from s1
			pc_map(s1->T2, &(s1->T), h);
			pc_map(s1->X2, &(s1->X), h);

			// Compute s1 + s2
			gt_mul(s->T2,s1->T2,s2->T2);
			gt_mul(s->X2,s1->X2,s2->X2);
			gt_copy(s->lambda, s2->lambda);
			s->degree = 2;
			break;
		case 5:		// s1 is of degree 2 and s2 of degree 1
			// Create a degree 2 fgp_tag from s2
			pc_map(s2->T2, &(s2->T), h);
			pc_map(s2->X2, &(s2->X), h);

			// Compute s1 + s2
			gt_mul(s->T2,s1->T2,s2->T2);
			gt_mul(s->X2,s1->X2,s2->X2);
			gt_copy(s->lambda, s1->lambda);
			s->degree = 2;
			break;
		case 6:		// Both are of degree 2
			gt_mul(s->T2,s2->T2,s1->T2);
			gt_mul(s->X2,s2->X2,s1->X2);
			gt_mul(s->lambda,s1->lambda,s2->lambda);
			s->degree = 2;
			break;
		default:
			error_hdl(-1,"The degrees of the tag don't add up");
			return 1;
	};
	
	g2_free(h);

	return 0;
}

int fgp_tag_zero(fgp_tag * s)
{
	s->degree = 1;
	g1_set_infty(&(s->T));
	g2_set_infty(&(s->U));
	g1_set_infty(&(s->X));
	g2_set_infty(&(s->Y));

	gt_zero(s->lambda);
	gt_zero(s->T2);
	gt_zero(s->X2);

	return 0;
}

int fgp_tag_print(fgp_tag *s)
{
	printf("Degree of Tag:%d\n", s->degree);

	switch (s->degree)
	{
		case 1:
			printf("T:\n");
			g1_print(&(s->T));
			printf("X:\n");
			g1_print(&(s->X));
			printf("U:\n");
			g2_print(&(s->U));
			printf("Y:\n");
			g2_print(&(s->Y));
			printf("lambda:\n");
			gt_print(s->lambda);
			break;
		case 2:
			printf("T:\n");
			gt_print(s->T2);
			printf("X:\n");
			gt_print(s->X2);
			printf("U:\n");
			printf("lambda:\n");
			gt_print(s->lambda);
			break;
		default:
			error_hdl(-1,"The degree of fgp_tag has to be either 1 or 2");
			return 1;
	};

	return 0;
}

int fgp_tag_write2(unsigned char * file, fgp_tag * s)
{	
	// Check degree
	if (s->degree != 2)
	{
		error_hdl(-1,"fgp_tag has to be of degree 1");
		return 1;
	}

	FILE * f = fopen(file , "w+");
	if (f == NULL)
	{
		error_hdl(-1,"error opening the file");
		return 1;
	}
	
	// Write degree
	long unsigned degree = 2;

	fwrite(&degree, sizeof(int),1, f);
	
	// Get the size in bytes
	int T_size = gt_size_bin(s->T2, 0);
	//printf("T_size:%d\n", T_size);
	int X_size = gt_size_bin(s->X2, 0);
	//printf("X_size:%d\n", X_size);
	int L_size = gt_size_bin(s->lambda, 0);
	//printf("L_size:%d\n", L_size);

	// Convert to byte
	uint8_t * T = malloc(sizeof(uint8_t)*T_size);
	uint8_t * X = malloc(sizeof(uint8_t)*X_size);
	uint8_t * L = malloc(sizeof(uint8_t)*L_size);

	gt_write_bin(T, T_size, s->T2, 0);
	gt_write_bin(X, X_size, s->X2, 0);
	gt_write_bin(L, L_size, s->lambda, 0);

	// Convert to mpz
	mpz_t T_m, X_m, L_m;
	mpz_init(T_m);
	mpz_init(X_m);
	mpz_init(L_m);

	mpz_import(T_m, T_size, 1, sizeof(uint8_t), 0, 0, T);
	mpz_import(X_m, X_size, 1, sizeof(uint8_t), 0, 0, X);
	mpz_import(L_m, L_size, 1, sizeof(uint8_t), 0, 0, L);

	// Write
	long unsigned int bitch = mpz_out_raw(f, T_m);
	//printf("Written:%lu\n", bitch);
	bitch = mpz_out_raw(f, X_m);
	//printf("Written:%lu\n", bitch);
	bitch = mpz_out_raw(f, L_m);
	//printf("Written:%lu\n", bitch);
	
	// Clean up
	mpz_clear(T_m);
	mpz_clear(X_m);
	mpz_clear(L_m);

	free(T);
	free(X);
	free(L);

	fclose(f);

	return 0;
}

int fgp_tag_write1(unsigned char * file, fgp_tag * s)
{	
	// Check degree
	if (s->degree != 1)
	{
		error_hdl(-1,"fgp_tag has to be of degree 1");
		return 1;
	}

	FILE * f = fopen(file , "w+");
	if (f == NULL)
	{
		error_hdl(-1,"error opening the file");
		return 1;
	}
	
	// Write degree
	int degree = 1;

	fwrite(&degree, sizeof(int),1, f);
	
	// Get the size in bytes
	int T_size = g1_size_bin(&(s->T), 0);
	int U_size = g2_size_bin(&(s->U), 0);
	int X_size = g1_size_bin(&(s->X), 0);
	int Y_size = g2_size_bin(&(s->Y), 0);
	//int L_size = gt_size_bin(s->lambda, 0);

	// Convert to byte
	uint8_t * T = malloc(sizeof(uint8_t)*T_size);
	uint8_t * U = malloc(sizeof(uint8_t)*U_size);
	uint8_t * X = malloc(sizeof(uint8_t)*X_size);
	uint8_t * Y = malloc(sizeof(uint8_t)*Y_size);
	//uint8_t * L = malloc(sizeof(uint8_t)*L_size);

	g1_write_bin(T, T_size, &(s->T), 0);
	g2_write_bin(U, U_size, &(s->U), 0);
	g1_write_bin(X, X_size, &(s->X), 0);
	g2_write_bin(Y, Y_size, &(s->Y), 0);
	//gt_write_bin(L, L_size, s->lambda, 0);

	// Convert to mpz
	mpz_t T_m, U_m, X_m, Y_m;//, L_m;
	mpz_init(T_m);
	mpz_init(U_m);
	mpz_init(X_m);
	mpz_init(Y_m);
	//mpz_init(L_m);

	mpz_import(T_m, T_size, 1, sizeof(uint8_t), 0, 0, T);
	mpz_import(U_m, U_size, 1, sizeof(uint8_t), 0, 0, U);
	mpz_import(X_m, X_size, 1, sizeof(uint8_t), 0, 0, X);
	mpz_import(Y_m, Y_size, 1, sizeof(uint8_t), 0, 0, Y);
	//mpz_import(L_m, L_size, 1, sizeof(uint8_t), 0, 0, L);

	// Write
	mpz_out_raw(f, T_m);
	mpz_out_raw(f, U_m);
	mpz_out_raw(f, X_m);
	mpz_out_raw(f, Y_m);
	//mpz_out_raw(f, L_m);
	
	// Clean up
	mpz_clear(T_m);
	mpz_clear(U_m);
	mpz_clear(X_m);
	mpz_clear(Y_m);
	//mpz_clear(L_m);

	free(T);
	free(U);
	free(X);
	free(Y);
	//free(L);

	fclose(f);

	return 0;
}

int fgp_tag_write(unsigned char * file, fgp_tag * s)
{
	switch (s->degree)
	{
		case 1:
			fgp_tag_write1(file, s);
			break;
		case 2:
			fgp_tag_write2(file, s);
			break;
		default:
			error_hdl(-1,"fgp_tag degree not accepted");
			return 1;
	};
	return 0;
}

int fgp_tag_read(fgp_tag * s, unsigned char * file)
{
	FILE *f = fopen(file, "r");
	if (f==NULL)
	{
		error_hdl(-1,"Error opening the file");
		return 1;
	}

	int degree;
	fread(&degree, sizeof(int), 1, f);
	fclose(f);

	switch(degree)
	{
		case 1:
			fgp_tag_read1(s, file);
			break;
		case 2:
			fgp_tag_read2(s, file);
			break;
		default:
			error_hdl(-1,"Degree not acceted");
			return 1;
	};

	return 0;
}

int fgp_tag_read1(fgp_tag * s, unsigned char * file)
{
	FILE * f = fopen(file , "r");
	if (f == NULL)
	{
		error_hdl(-1,"error opening the file");
		return 1;
	}
	
	int degree;
	fread(&degree, sizeof(int), 1, f);

	if (degree != 1)
	{
		error_hdl(-1,"Degree of sigma should be 1");
		return 1;
	}
	

	long unsigned int size;
	s->degree = 1;

	// Read into mpz integers
	mpz_t T_m, U_m, X_m, Y_m;//, L_m;
	mpz_init(T_m);
	mpz_init(U_m);
	mpz_init(X_m);
	mpz_init(Y_m);
	//mpz_init(L_m);

	// Read and convert
	size = mpz_inp_raw(T_m, f);
	//printf("%lu\n", size);
	uint8_t * T = malloc(sizeof(uint8_t)*(size-4));		// 4 bytes of information 
	/*
	for (int i = 0; i < size; ++i)
	{
		printf("%d ", (int) T[i]);
	}
	printf("\n");
	*/
	mpz_export(T, &size, 1, sizeof(uint8_t), 0, 0, T_m);
	g1_read_bin(&(s->T), T, size);
	//g1_print(&(s->T));

	size = mpz_inp_raw(U_m, f);
	uint8_t * U = malloc(sizeof(uint8_t)*(size-4));
	mpz_export(U, &size, 1, sizeof(uint8_t), 0, 0, U_m);
	g2_read_bin(&(s->U), U, size);
	//g2_print(&(s->U));

	size = mpz_inp_raw(X_m, f);
	uint8_t * X = malloc(sizeof(uint8_t)*(size-4));
	mpz_export(X, &size, 1, sizeof(uint8_t), 0, 0, X_m);
	g1_read_bin(&(s->X), X, size);
	//g1_print(&(s->X));

	size = mpz_inp_raw(Y_m, f);
	uint8_t * Y = malloc(sizeof(uint8_t)*(size-4));
	mpz_export(Y, &size, 1, sizeof(uint8_t), 0, 0, Y_m);
	g2_read_bin(&(s->Y), Y, size);

	/*
	size = mpz_inp_raw(L_m, f);
	uint8_t * L = malloc(sizeof(uint8_t)*(384));
	mpz_export(L, &size, 1, sizeof(uint8_t), 0, 0, L_m);
	printf("%lu\n",size );
	gt_read_bin(s->lambda, L, size);
	*/
	
	gt_set_unity(s->lambda);

	// Clean up

	mpz_clear(T_m);
	mpz_clear(U_m);
	mpz_clear(X_m);
	mpz_clear(Y_m);
	//mpz_clear(L_m);

	free(T);
	free(U);
	free(X);
	free(Y);
	//free(L);

	fclose(f);

	return 0;
}

int fgp_tag_read2(fgp_tag * s, unsigned char * file)
{
	FILE * f = fopen(file , "r");
	if (f == NULL)
	{
		error_hdl(-1,"error opening the file");
		return 1;
	}
	
	int degree;
	fread(&degree, sizeof(int), 1, f);

	if (degree != 2)
	{
		error_hdl(-1,"Degree of sigma should be 2");
		return 1;
	}
	
	long unsigned int size;
	s->degree = 2;

	// Read into mpz integers
	mpz_t T_m, X_m, L_m;
	mpz_init(T_m);
	mpz_init(X_m);
	mpz_init(L_m);

	// Read and convert
	size = mpz_inp_raw(T_m, f);
	//printf("Read:%lu\n", size);
	//printf("%lu\n", size);
	uint8_t * T = malloc(sizeof(uint8_t)*(size-4));		// 4 bytes of information 
	mpz_export(T, &size, 1, sizeof(uint8_t), 0, 0, T_m);
	//printf("mpz size:%lu\n", size);
	gt_read_bin(s->T2, T, size);
	//g1_print(&(s->T));

	size = mpz_inp_raw(X_m, f);
	//printf("Read:%lu\n", size);
	uint8_t * X = malloc(sizeof(uint8_t)*(size-4));
	mpz_export(X, &size, 1, sizeof(uint8_t), 0, 0, X_m);
	//printf("mpz size:%lu\n", size);
	gt_read_bin(s->X2, X, size);
	//g1_print(&(s->X));

	
	size = mpz_inp_raw(L_m, f);
	//printf("Read:%lu\n", size);
	uint8_t * L = malloc(sizeof(uint8_t)*(384));
	mpz_export(L, &size, 1, sizeof(uint8_t), 0, 0, L_m);
	//printf("mpz size:%lu\n", size);
	gt_read_bin(s->lambda, L, size);

	// Clean up

	mpz_clear(T_m);
	mpz_clear(X_m);
	mpz_clear(L_m);

	free(T);
	free(X);
	free(L);

	fclose(f);

	return 0;
}
