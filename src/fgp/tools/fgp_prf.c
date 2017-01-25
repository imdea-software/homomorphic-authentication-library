#include "fgp_prf.h"

int decouple(bn_st * a, bn_st * b, char * input, uint8_t * key, int key_size)
{
	int in_size;
	uint8_t * byte_input  = char_to_uint8_t(&in_size, input);
	if (byte_input == NULL)
	{
		error_hdl(-1,"Error with the conversion to byte of the input");
		return 1;
	}

	// Encrypt the input
	uint8_t * hash_msg = malloc(sizeof(uint8_t)*HASH_LEN);			// HASH_LEN is the output length of the hash fucntion used

	if (prf(hash_msg, HASH_LEN, byte_input, in_size, key, key_size))
	{
		error_hdl(1,"Error with the encryption");
		return 1;
	}

	free(byte_input);


	int byte_number = EXP_LEN/8 + 1*(EXP_LEN%8 != 0);

	uint8_t * exp_a = malloc(sizeof(uint8_t)*byte_number);
	uint8_t * exp_b = malloc(sizeof(uint8_t)*byte_number);
	

	// Separate the input in two

	if (separate(exp_a, exp_b, hash_msg, HASH_LEN))
	{
		error_hdl(1,"Error with the separation of the hash");
		return 1;
	}


	// Expand the outputs to the appropriate size

	if (expand(exp_a, exp_a, HASH_LEN/2, EXP_LEN))		// The relevant size of exp_a is HASH_LEN/2
	{
		error_hdl(1,"Error with the expansion of the first output");
		return 1;
	}


	if (expand(exp_b, exp_b, HASH_LEN/2, EXP_LEN))		// The relevant size of exp_b is HASH_LEN/2
	{
		error_hdl(1,"Error with the expansion of the second output");
		return 1;
	}


	// Reduce the outputs with the order of the groups as modulus

	bn_t bn_exp_a, bn_exp_b;				// The expanded out in bn form
	bn_t order;								// Where the order of the groups (aka modulus) is store

	bn_new_size(bn_exp_a, (EXP_LEN)/DIGIT +1);		// Size in digits -> RELIC_DIGS*factor
	bn_new_size(bn_exp_b, (EXP_LEN)/DIGIT +1);

	bn_read_bin(bn_exp_a, exp_a, byte_number);		// Convert the expanded outputs in multiprecision integers
	bn_read_bin(bn_exp_b, exp_b, byte_number);


	bn_new_size(order, RELIC_DIGS);			// Size in digits of a prime field element
	g1_get_ord(order);					// Inizialize the order variable


	// Calculate the modulus

	bn_mod_basic(a, bn_exp_a, order);
	bn_mod_basic(b, bn_exp_b, order);


	// Clean the multi precision integers

	free(hash_msg);
	free(exp_a);
	free(exp_b);
	bn_clean(bn_exp_a);
	bn_clean(bn_exp_b);
	bn_clean(order);
	return 0;
}