#include "bn_ext.h"

int bn_mul_mod(bn_st * out, bn_st * in1, bn_st * in2, bn_st * modulus)
{
	// Temporary variable in which to store the reuslt of the addition
	// before appplying the modular reduction
	bn_st temp;
	bn_new_size(&temp, 2*RELIC_DIGS);

	// Multiply
	bn_mul_basic(&temp,in1,in2);
	// Reduce
	bn_mod_basic(out, &temp, modulus);
	bn_clean(&temp);
	return 0;
}

int bn_mult_arrays(bn_st * bn_out, bn_st * bn_array1, bn_st * bn_array2, int size)
{
	for (int i = 0; i < size; ++i)
	{
		switch (MULT_TYPE)
		{
			case 'b':		// Basic
				bn_mul_basic(&bn_out[i], &bn_array1[i], &bn_array2[i]);
				break;
			case 'c':		// Comba
				bn_mul_comba(&bn_out[i], &bn_array1[i], &bn_array2[i]);
				break;
			case 'k':		// Karatsuba
				bn_mul_karat(&bn_out[i], &bn_array1[i], &bn_array2[i]);
				break;
			default:
				error_hdl(-1,"Multiplication type not recognized");
				return 1;
		}
	}
	return 0;
}

int bn_mult_arrays_mod(bn_st * bn_out, bn_st * bn_array1, bn_st * bn_array2, int size, bn_st * modulus)
{
	for (int i = 0; i < size; ++i)
	{
		switch (MULT_TYPE)
		{
			case 'b':		// Basic
				bn_mul_basic(&bn_out[i], &bn_array1[i], &bn_array2[i]);
				break;
			case 'c':		// Comba
				bn_mul_comba(&bn_out[i], &bn_array1[i], &bn_array2[i]);
				break;
			case 'k':		// Karatsuba
				bn_mul_karat(&bn_out[i], &bn_array1[i], &bn_array2[i]);
				break;
			default:
				error_hdl(-1,"Multiplication type not recognized");
				return 1;
		}
		switch (MOD_TYPE)
		{
			case 'b':		// Basic
				bn_mod_basic(&bn_out[i], &bn_out[i], modulus);
				break;
			case 'c':
				break;
			case 'k':
				break;
			default:
				error_hdl(-1,"Modular type not recognized");
				return 1;	
		}
		
	}
	return 0;
}

int bn_add_mod(bn_st * out, bn_st * in1, bn_st * in2, bn_st * modulus)
{
	// Temporary variable in which to store the reuslt of the addition
	// before appplying the modular reduction
	bn_st temp;
	bn_new_size(&temp, RELIC_DIGS+1);
	// Addition
	bn_add(&temp,in1,in2);
	// Modular reduction
	bn_mod_basic(out, &temp, modulus);
	bn_clean(&temp);
	return 0;
}

int bn_sub_mod(bn_st * out, bn_st * in1, bn_st * in2, bn_st * modulus)
{
	if (bn_cmp(in1,in2) == CMP_GT)
	{
		// If in1 > in2 we just subtract
		bn_sub(out,in1,in2);
	}
	else
	{
		// Otherwise compute mod + in1 - in2
		bn_add(out, modulus, in1);
		bn_sub(out,out,in2);
	}
	return 0;
}

int bn_sqr_mod(bn_st * out, bn_st * in, bn_st * modulus)
{
	bn_t temp;
	bn_new_size(temp, 2*RELIC_DIGS);
	bn_sqr_basic(temp, in);
	bn_mod_basic(out,temp, modulus);
	bn_clean(temp);
	return 0;
}

int bn_add_array_mod(bn_st * bn_out, bn_st * bn_array, int size, bn_st * modulus)
{
	for (int i = 0; i < size; ++i)
	{
		bn_add(bn_out, &bn_array[i], bn_out);		// Add all the values
		bn_mod_basic(bn_out, bn_out, modulus);		// Reduce at each step
	}
	return 0;
}

int bn_add_array(bn_st * bn_out, bn_st * bn_array, int size)
{
	for (int i = 0; i < size; ++i)
	{
		bn_add(bn_out, &bn_array[i], bn_out);		// Add all the values
	}
	return 0;
}

int inverse(bn_st * inv_in, bn_st * in, bn_st * modulus, int sizeof_mod)
{
	// Compute the inverse of in using an extended Euclidian Algorithm
	// This cooresponds to the formula out = inv_in*in + useless*modulus
	// Where, since mod is prime, out should be equal to 1 and 1 = in*inv_in
	// Otherwise throws an error

	bn_mod_basic(in,in,modulus);

	bn_st out, useless;
	bn_new_size(&out, 2*sizeof_mod+1);
	bn_new_size(&useless, sizeof_mod);

	bn_gcd_ext(&out, inv_in, &useless, in, modulus);

	// Check if out is actually one

	bn_st is_one;						
	bn_new_size(&is_one, sizeof_mod);
	bn_set_2b(&is_one, 0);		// Set an integer to one


	if (bn_cmp(&out, &is_one) != CMP_EQ)
	{
		error_hdl(-1,"The modulus is not prime, can't calculate the inverse");
		return 1;
	}

	bn_clean(&is_one);
	bn_clean(&out);
	bn_clean(&useless);

	return 0;
}

int bn_new_array(bn_st * array, int size)
{
	for (int i = 0; i < size; ++i)
	{
		bn_new_size(array + i, RELIC_DIGS);
	}
	return 0;
}

int bn_free_array(bn_st * array, int size)
{
	for (int i = size-1; i >= 0; --i)
	{
		bn_clean(array + i);
	}
	return 0;
}

int bn_zero_array(bn_t * array, int size)
{
	for (int i = 0; i < size; ++i)
	{
		bn_zero(array[i]);
	}
	return 0;
}

int mpz2bn(bn_t out, mpz_t in)		
{
	int size = mpz_sizeinbase(in, 10) + 2;		// + 2 bytes for minus sign and NULL
	char * temp = malloc(sizeof(char)*size);

	mpz_get_str(temp, 10, in);
	bn_zero(out);
	bn_read_str(out, temp, size, 10);

	free(temp);
	return 0;
}

int fmpz2bn(bn_t out, fmpz_t in)		
{
	mpz_t temp;
	mpz_init(temp);

	fmpz_get_mpz(temp , in);

	mpz2bn(out, temp);

	mpz_clear(temp);

	return 0;
}

int fq2bn(bn_t out, fq_t in, fq_ctx_t ctx)
{
	fmpz_t temp;
	fmpz_init(temp);

	fmpz_set_str(temp, fq_get_str_pretty(in, ctx), 10);

	fmpz2bn(out, temp);

	fmpz_clear(temp);
	return 0;
}

int bn2mpz(mpz_t out, bn_t in)
{
	int size = bn_size_str(in, 10);
	char * temp = malloc(sizeof(char)*size);

	bn_write_str(temp, size, in, 10);
	mpz_set_str(out,temp, 10);

	free(temp);
	return 0;
}

int bn2fmpz(fmpz_t out, bn_t in)
{
	mpz_t temp;
	mpz_init(temp);

	bn2mpz(temp, in);

	fmpz_set_mpz(out , temp);

	mpz_clear(temp);
	return 0;
}

int bn2fq(fq_t out, bn_t in, fq_ctx_t ctx)
{
	fmpz_t temp;
	fmpz_init(temp);
	
	bn2fmpz(temp, in);
	fq_set_fmpz(out, temp, ctx);

	fmpz_clear(temp);
	return 0;
}

int affine(bn_st * y, bn_st  * a, bn_st * x, bn_st * b, bn_st * mod)
{
	bn_mul_mod(y, a, x, mod);
	bn_add_mod(y, y, b, mod);
	return 0;
}

int g1_add_norm(g1_t out, g1_t in1, g1_t in2)
{
	g1_add(out, in1, in2);
	g1_norm(out, out);
	return 0;
}

int g2_add_norm(g2_t out, g2_t in1, g2_t in2)
{
	g2_add(out, in1, in2);
	g2_norm(out, out);
	return 0;
}

int int2uint8_t(uint8_t * out, int in)
{
	int num = 1;
	// Output has to be big endian
	if (*(char *)&num == 1)			// Little endian
	{
		out[3] = (uint8_t)(in >>  0);
	    out[2] = (uint8_t)(in >>  8);
	    out[1] = (uint8_t)(in >> 16);
	    out[0] = (uint8_t)(in >> 24);
	}
	else							// Big endian
	{
		out[3] = (uint8_t)(in <<  0);
	    out[2] = (uint8_t)(in <<  8);
	    out[1] = (uint8_t)(in << 16);
	    out[0] = (uint8_t)(in << 24);
	}
    return 0;
}

int uint8_t2int(int * out, uint8_t * in)
{
	// The byte array is in big endian
	int num = 1;
	if (*(char *)&num == 1)			// Little endian
	{
		*out = (int) in[0];
		*out = (*out << 8);

		*out += (int) in[1];
		*out = (*out << 8);

		*out += (int) in[2];
		*out = (*out << 8);

		*out += (int) in[3];
	}
	else							// Big endian
	{
		*out = (int) in[0];
		*out = (*out >> 8);

		*out += (int) in[1];
		*out = (*out >> 8);

		*out += (int) in[2];
		*out = (*out >> 8);

		*out += (int) in[3];
	}
	

    return 0;
}

int int2bn(bn_t out, int in)
{
	uint8_t * temp = malloc(sizeof(uint8_t)*4);
	int2uint8_t(temp, in);

	bn_read_bin(out, temp, 4);
	free(temp);

	return 0;
}

int bn2int(int * out, bn_t in)
{
	uint8_t * temp = malloc(sizeof(uint8_t)*4);
	bn_write_bin(temp, 4, in);
	uint8_t2int(out, temp);
	free(temp);

	return 0;
}

int sint2bn(bn_t out, int in, int size)
{
	char * str = malloc(sizeof(char)*size);
	snprintf(str, size, "%d", in);
	bn_read_str(out, str, size, 10);
	free(str);
	return 0;
}

int mod_set_fmpz(fmpz_t mod)
{
	bn_t order;
	bn_new(order);
	g1_get_ord(order);

	fmpz_init(mod);

	bn2fmpz(mod, order);

	bn_clean(order);

	return 0;
}