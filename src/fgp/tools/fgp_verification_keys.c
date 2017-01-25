#include "fgp_verification_keys.h"

int fgp_vkf_new(fgp_vkf * r)
{
	// Assign the degree
	r->degree = 1;

	// Assign the modulus
	bn_new_size(&(r->mod), RELIC_DIGS);
	g1_get_ord(&(r->mod));
	
	// Allocate memory 
	bn_new_size(&(r->C), RELIC_DIGS);
	bn_new_size(&(r->ABcoef), RELIC_DIGS);
	bn_new_size(&(r->A2coef), RELIC_DIGS);
	bn_new_size(&(r->B2coef), RELIC_DIGS);
	bn_new_size(&(r->Acoef), RELIC_DIGS);
	bn_new_size(&(r->Bcoef), RELIC_DIGS);

	return 0;
}

int fgp_vkf_copy(fgp_vkf *r_out, fgp_vkf *r_in)
{
	switch (r_in->degree)
	{
		case 2:
			bn_copy(&(r_out->B2coef), &(r_in->B2coef));
			bn_copy(&(r_out->ABcoef), &(r_in->ABcoef));
			bn_copy(&(r_out->A2coef), &(r_in->A2coef));
		case 1:
			bn_copy(&(r_out->Acoef), &(r_in->Acoef));
			bn_copy(&(r_out->Bcoef), &(r_in->Bcoef));
			break;
		default:
			error_hdl(-1,"fgp_vkf degree not accepted");
			return 1;
	};
	// Constant
	bn_copy(&(r_out->C), &(r_in->C));
	r_out->degree = r_in->degree;	
	return 0;
}

int fgp_vkf_free(fgp_vkf *r)
{
	bn_clean(&(r->mod));
	bn_clean(&(r->C));
	bn_clean(&(r->ABcoef));
	bn_clean(&(r->A2coef));
	bn_clean(&(r->B2coef));

	bn_clean(&(r->Acoef));
	bn_clean(&(r->Bcoef));

	free(r);
	return 0;
}

int fgp_vkf_clean(fgp_vkf *r)
{
	bn_clean(&(r->mod));
	bn_clean(&(r->C));
	bn_clean(&(r->ABcoef));
	bn_clean(&(r->A2coef));
	bn_clean(&(r->B2coef));

	bn_clean(&(r->Acoef));
	bn_clean(&(r->Bcoef));
	return 0;
}

int fgp_vkf_bilinear(fgp_vkf *r, bn_st * _Acoef, bn_st *_Bcoef)
{
	r->degree = 1;
	bn_copy(&(r->Acoef),_Acoef);
	bn_copy(&(r->Bcoef),_Bcoef);
	bn_zero(&(r->ABcoef));
	bn_zero(&(r->A2coef));
	bn_zero(&(r->B2coef));
	return 0;
}


int fgp_vkf_const_mult(fgp_vkf * r_out, fgp_vkf * r_in, bn_st * constant)
{	
	bn_mul_mod(&(r_out->C), &(r_in->C), constant, &(r_out->mod));
	bn_mul_mod(&(r_out->Acoef), &(r_in->Acoef), constant, &(r_out->mod));
	bn_mul_mod(&(r_out->Bcoef), &(r_in->Bcoef), constant, &(r_out->mod));
	if (r_in->degree == 2)
	{
		bn_mul_mod(&(r_out->ABcoef), &(r_in->ABcoef), constant, &(r_out->mod));
		bn_mul_mod(&(r_out->B2coef), &(r_in->B2coef), constant, &(r_out->mod));
		bn_mul_mod(&(r_out->A2coef), &(r_in->A2coef), constant, &(r_out->mod));
	}
	r_out->degree = r_in->degree;
	return 0;
}



int fgp_vkf_add(fgp_vkf * r_out, fgp_vkf * r1, fgp_vkf * r2)
{
	bn_add_mod(&(r_out->C), &(r1->C), &(r2->C), &(r_out->mod));
	bn_add_mod(&(r_out->Acoef), &(r1->Acoef), &(r2->Acoef), &(r_out->mod));
	bn_add_mod(&(r_out->Bcoef), &(r1->Bcoef), &(r2->Bcoef), &(r_out->mod));
	// Add the degree 2 coefficients
	bn_add_mod(&(r_out->ABcoef), &(r1->ABcoef), &(r2->ABcoef), &(r_out->mod));
	bn_add_mod(&(r_out->B2coef), &(r1->B2coef), &(r2->B2coef), &(r_out->mod));
	bn_add_mod(&(r_out->A2coef), &(r1->A2coef), &(r2->A2coef), &(r_out->mod));

	r_out->degree = (r1->degree)*(r1->degree >= r2->degree) + (r2->degree)*(r1->degree < r2->degree);
	return 0;
}

int fgp_vkf_mul(fgp_vkf *r_out, fgp_vkf *r1, fgp_vkf *r2)
{
	if (r1->degree + r2->degree > 2)
	{
		error_hdl(-1,"Both inputs have to be of degree 1 to be multiplied");
		return 1;
	}
	// Set the degree 1 coefs to zero
	bn_zero(&(r_out->Acoef));
	bn_zero(&(r_out->Bcoef));

	// Set a temporary integer used to store an intermidiate value
	bn_t temp;
	bn_new_size(temp, RELIC_DIGS);

	// r1 = (C_1 + U_1*A + V_1*B)
	// r2 = (C_2 + U_2*A + V_2*B)
	// r_out = (U_1*U_2)*A^2 + (U_1*V_2 + U_2*V_1)*AB + (V_1*V_2)*B^2 + (C_1*U_2 + U_1*C_2)*A + (C_1*V_2 + C_2*V_1)*B + C_1*C_2

	// U1*U2
	bn_mul_mod(&(r_out->A2coef), &(r1->Acoef), &(r2->Acoef), &(r_out->mod));
	// V1*V2
	bn_mul_mod(&(r_out->B2coef), &(r1->Bcoef), &(r2->Bcoef), &(r_out->mod));
	
	// U1*V2
	bn_mul_mod(&(r_out->ABcoef), &(r1->Acoef), &(r2->Bcoef), &(r_out->mod));
	// U2*V1
	bn_mul_mod(temp, &(r1->Bcoef), &(r2->Acoef), &(r_out->mod));
	// U1*V2 + U2*V1
	bn_add_mod(&(r_out->ABcoef), &(r_out->ABcoef), temp, &(r_out->mod));

	// C_1*U_2 + U_1*C_2
	bn_mul_mod(&(r_out->Acoef), &(r1->Acoef), &(r2->C), &(r1->mod));
	bn_mul_mod(temp, &(r1->C), &(r2->Acoef), &(r1->mod));
	bn_add_mod(&(r_out->Acoef), &(r_out->Acoef), temp, &(r1->mod));

	// C_1*V_2 + C_2*V_1
	bn_mul_mod(&(r_out->Bcoef), &(r1->Bcoef), &(r2->C), &(r1->mod));
	bn_mul_mod(temp, &(r1->C), &(r2->Bcoef), &(r1->mod));
	bn_add_mod(&(r_out->Bcoef), &(r_out->Bcoef), temp, &(r1->mod));

	// C_1*C_2
	bn_mul_mod(&(r_out->C), &(r1->C), &(r2->C), &(r1->mod));

	// Set the output degree
	r_out->degree = 2;
	bn_clean(temp);

	return 0;
}

int fgp_vkf_eval(bn_st * output, fgp_vkf * r, bn_st * in1, bn_st * in2)
{
	bn_st temp;
	bn_new_size(&temp, RELIC_DIGS);

	// Initialize out to zero
	bn_zero(output);

	// coefA * in1
	bn_mul_mod(&temp, in1, &(r->Acoef), &(r->mod));
	bn_add_mod(output, output, &temp, &(r->mod));

	// coefB * in2
	bn_mul_mod(&temp, in2, &(r->Bcoef), &(r->mod));
	bn_add_mod(output, output, &temp, &(r->mod));

	// If the degree is 2, compute the degree 2 terms
	if (r->degree == 2)
	{
		// coefAB * in1 * in2
		bn_mul_mod(&temp, in1, in2, &(r->mod));
		bn_mul_mod(&temp, &temp, &(r->ABcoef), &(r->mod));
		bn_add_mod(output, output, &temp, &(r->mod));

		// coefA2 * in1^2
		bn_mul_mod(&temp, in1, in1, &(r->mod));
		bn_mul_mod(&temp, &temp, &(r->A2coef), &(r->mod));
		bn_add_mod(output, output, &temp, &(r->mod));

		// coefb2 * in2^2
		bn_mul_mod(&temp, in2, in2, &(r->mod));
		bn_mul_mod(&temp, &temp, &(r->B2coef), &(r->mod));
		bn_add_mod(output, output, &temp, &(r->mod));
	}

	// Add the constant
	bn_add_mod(output, output, &(r->C), &(r->mod));
	
	return 0;
}

int fgp_vkf_set(fgp_vkf * r, fgp_private_key * key, char * L)
{
	if (decouple(&(r->Acoef), &(r->Bcoef), L, key->K1, KEY_SIZE))
	{
		error_hdl(-1,"Error with the prf on L");
		return 1;
	}
	// Set degree to 1
	r->degree = 1;
	return 0;
}

int fgp_vkf_const_add(fgp_vkf * r_out, fgp_vkf * r_in, bn_t constant)
{
	fgp_vkf_copy(r_out, r_in);
	bn_add_mod(&(r_out->C), &(r_in->C), constant, &(r_in->mod));

	return 0;
}

int fgp_vkf_const_sub(fgp_vkf * r_out, fgp_vkf * r_in, bn_t constant)
{
	fgp_vkf_copy(r_out, r_in);
	bn_sub_mod(&(r_out->C), &(r_in->C), constant, &(r_in->mod));

	return 0;
}

int fgp_vkf_zero(fgp_vkf *r)
{
	bn_zero(&(r->C));
	bn_zero(&(r->ABcoef));
	bn_zero(&(r->A2coef));
	bn_zero(&(r->B2coef));

	bn_zero(&(r->Acoef));
	bn_zero(&(r->Bcoef));
	r->degree = 1;
	return 0;
}

int fgp_vkf_print(fgp_vkf *r)
{
	printf("fgp_vkf of degree: %d\n", r->degree);
	printf("\tConstant:\t");
	bn_print(&(r->C));
	printf("\tA coefficient\t");
	bn_print(&(r->Acoef));
	printf("\tB coefficient\t");
	bn_print(&(r->Bcoef));
	if (r->degree == 2)
	{
		printf("\tAB coefficient\t");
		bn_print(&(r->ABcoef));
		printf("\tA2 coefficient\t");
		bn_print(&(r->A2coef));
		printf("\tB2 coefficient\t");
		bn_print(&(r->B2coef));
	}
	return 0;
}


int fgp_vkf_write1(unsigned char * file, fgp_vkf * r)
{
	FILE *f = fopen(file, "w+");
	if (f==NULL)
	{
		error_hdl(-1,"Error opening the file");
		return 1;
	}

	if (r->degree != 1)
	{
		error_hdl(-1,"Degree of verification key should be 1");
		return 1;
	}

	fwrite(&(r->degree), sizeof(int),1, f);

	mpz_t c_t, a_t, b_t; //* mpz_array = malloc(sizeof(mpz_t)*size);
	mpz_init2(a_t, FP_BITS);
	mpz_init2(b_t, FP_BITS);
	mpz_init2(c_t, FP_BITS);

	bn2mpz(c_t, &(r->C));
	bn2mpz(a_t, &(r->Acoef));
	bn2mpz(b_t, &(r->Bcoef));
	
	mpz_out_raw(f, c_t);	
	mpz_out_raw(f, a_t);
	mpz_out_raw(f, b_t);

	mpz_clear(c_t);
	mpz_clear(a_t);
	mpz_clear(b_t);

	fclose(f);
	return 0;
}

int fgp_vkf_write(unsigned char * file, fgp_vkf * r)
{
	switch(r->degree)
	{
		case 1:
			fgp_vkf_write1(file, r);
			break;
		case 2:
			fgp_vkf_write2(file, r);
			break;
		default:
			error_hdl(-1,"Degree not acceted");
			return 1;
	};
	return 0;
}

int fgp_vkf_write2(unsigned char * file, fgp_vkf * r)
{
	FILE *f = fopen(file, "w+");
	if (f==NULL)
	{
		error_hdl(-1,"Error opening the file");
		return 1;
	}

	if (r->degree != 2)
	{
		error_hdl(-1,"Degree of verification key should be 2");
		return 1;
	}

	fwrite(&(r->degree), sizeof(int),1, f);

	mpz_t c_t, a_t, b_t, ab_t, a2_t, b2_t;
	mpz_init2(c_t, FP_BITS);
	mpz_init2(a_t, FP_BITS);
	mpz_init2(b_t, FP_BITS);
	mpz_init2(ab_t, FP_BITS);
	mpz_init2(a2_t, FP_BITS);
	mpz_init2(b2_t, FP_BITS);

	bn2mpz(c_t, &(r->C));
	bn2mpz(a_t, &(r->Acoef));
	bn2mpz(b_t, &(r->Bcoef));
	bn2mpz(ab_t, &(r->ABcoef));
	bn2mpz(a2_t, &(r->A2coef));
	bn2mpz(b2_t, &(r->B2coef));
	
	mpz_out_raw(f, c_t);	
	mpz_out_raw(f, a_t);
	mpz_out_raw(f, b_t);
	mpz_out_raw(f, ab_t);
	mpz_out_raw(f, a2_t);
	mpz_out_raw(f, b2_t);

	mpz_clear(c_t);
	mpz_clear(a_t);
	mpz_clear(b_t);
	mpz_clear(ab_t);
	mpz_clear(a2_t);
	mpz_clear(b2_t);

	fclose(f);
	return 0;
}

int fgp_vkf_read(fgp_vkf * r, unsigned char * file)
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
			fgp_vkf_read1(r, file);
			break;
		case 2:
			fgp_vkf_read2(r, file);
			break;
		default:
			error_hdl(-1,"Degree not acceted");
			return 1;
	};

	return 0;
}


int fgp_vkf_read1(fgp_vkf * r, unsigned char * file)
{
	FILE *f = fopen(file, "r");
	if (f==NULL)
	{
		error_hdl(-1,"Error opening the file");
		return 1;
	}

	int degree;
	fread(&degree, sizeof(int), 1, f);

	if (degree != 1)
	{
		error_hdl(-1,"Degree of verification key should be 1");
		return 1;
	}
	
	mpz_t c_t, a_t, b_t; 
	mpz_init2(a_t, FP_BITS);
	mpz_init2(b_t, FP_BITS);
	mpz_init2(c_t, FP_BITS);
	
	mpz_inp_raw(c_t, f);
	mpz_inp_raw(a_t, f);
	mpz_inp_raw(b_t, f);

	mpz2bn(&(r->C), c_t);
	mpz2bn(&(r->Acoef), a_t);
	mpz2bn(&(r->Bcoef), b_t);

	r->degree = 1;
	
	mpz_clear(c_t);
	mpz_clear(a_t);
	mpz_clear(b_t);


	fclose(f);
	return 0;
}

int fgp_vkf_read2(fgp_vkf * r, unsigned char * file)
{
	FILE *f = fopen(file, "r");
	if (f==NULL)
	{
		error_hdl(-1,"Error opening the file");
		return 1;
	}

	int degree;
	fread(&degree, sizeof(int), 1, f);

	if (degree != 2)
	{
		error_hdl(-1,"Degree of verification key should be 2");
		return 1;
	}
	
	mpz_t c_t,a_t, b_t, ab_t, a2_t, b2_t;
	mpz_init2(c_t, FP_BITS);
	mpz_init2(a_t, FP_BITS);
	mpz_init2(b_t, FP_BITS);
	mpz_init2(ab_t, FP_BITS);
	mpz_init2(a2_t, FP_BITS);
	mpz_init2(b2_t, FP_BITS);
	
	mpz_inp_raw(c_t, f);
	mpz_inp_raw(a_t, f);
	mpz_inp_raw(b_t, f);
	mpz_inp_raw(ab_t, f);
	mpz_inp_raw(a2_t, f);
	mpz_inp_raw(b2_t, f);

	mpz2bn(&(r->C), c_t);
	mpz2bn(&(r->Acoef), a_t);
	mpz2bn(&(r->Bcoef), b_t);
	mpz2bn(&(r->ABcoef), ab_t);
	mpz2bn(&(r->A2coef), a2_t);
	mpz2bn(&(r->B2coef), b2_t);

	r->degree = 2;
	
	mpz_clear(c_t);
	mpz_clear(a_t);
	mpz_clear(b_t);
	mpz_clear(ab_t);
	mpz_clear(a2_t);
	mpz_clear(b2_t);

	fclose(f);
	return 0;
}
