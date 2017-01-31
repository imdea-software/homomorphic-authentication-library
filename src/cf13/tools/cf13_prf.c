#include "cf13_prf.h"

cf13_prf(fq_t r, fq_ctx_t ctx, char * input, uint8_t * key, int key_size)
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
	
	//reconstruct sec_param from the PRF key size
	int sec_param = key_size*8;
	//the size of the expanded output is |q|+sec_param = 2*sec_param
	int exp_len = 2*sec_param;
	int byte_number = exp_len/8 + 1*(exp_len%8 != 0);
	
	uint8_t * exp_out = malloc(sizeof(uint8_t)*byte_number);
	
	
	// Expand the outputs to the appropriate size
	
	if (expand(exp_out, hash_msg, HASH_LEN, exp_len))
	{
		error_hdl(1,"Error with the expansion of the output");
		return 1;
	}
	free(hash_msg);
	
	
	// Reduce the output with the modulus
	
	//first convert from uint8_t to char
	char * exp_out_c = malloc(sizeof(char)*(byte_number*2+1));
	uint8_t_to_hex(exp_out_c, exp_out, byte_number);
	exp_out_c[byte_number*2] = NULL;
	
	//convert into an integer
	fmpz_t temp;
	fmpz_init(temp);
	fmpz_set_str(temp, exp_out_c, 16);
	
	//reduce mod q
	
	//set the output
	fq_set_fmpz(r, temp, ctx);
	
	/** DEBUGGING
	printf("PRF output:\n\tchar: %s\n\tfmpz: ", exp_out_c);
	fmpz_print(temp);
	printf("\n\tfq: ");
	fq_print_pretty(r, ctx);
	printf("\n");
	*/
	
	// Clean the multi precision integers
	
	free(exp_out_c);
	fmpz_clear(temp);
	return 0;
}
