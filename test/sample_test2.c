#include <fgp.h>

#include "function.h"

int main(int argc, char **argv)
{
	/**
	 * Usage example of the library with the compiler for homomorphic computations.
	 * It assumes the functions to be already generated and available in function.h
	**/

	// INITIALIZE THE FGP SCHEME
	
	if (fgp_init())
	{
		printf("Testing FAILED\n");
		printf("Problem initializing the library\n");
		return 1;
	}

	// SET UP THE TESTING VARIABLES

	// Get the order of the elliptic curve groups

	bn_t order;
	bn_new(order);
	g1_get_ord(order);

	// FUNCTION COEFFICIENTS GENERATION
	// Create some coefficients to use in a function

	int size = 100;
	
	bn_t * coefficients = malloc(sizeof(bn_t)*size);
	for (int i = 0; i < size; ++i)
	{
		bn_new(coefficients[i]);
		bn_rand_mod(coefficients[i], order);
	}

	// INPUT GENERATION
	// Create some messages (polynomials of degree 1 or less in Y)
	// As well as a unique string identifier for these ciphertexts: delta
	// And a unique string identifier for each ciphertext within this data set: L[i]

	char delta[19] = "data_set_identifier";

	char ** L = malloc(sizeof(char*)*size);

	fgp_msg ** msgs = malloc(sizeof(fgp_msg *)*size);

	for (int i = 0; i < size; ++i)
	{
		msgs[i] = malloc(sizeof(*msgs[i]));

		L[i] = malloc(sizeof(char)*MAX_STR_SIZE);
		label(L[i], i);		// Creates a string containing the number i

		fgp_msg_new(msgs[i]);
		fgp_msg_rand(msgs[i], 20, 1);
	}

	// COMPUTATION

	fgp_msg * c_out = malloc(sizeof(*c_out));
	fgp_msg_new(c_out);

	func_fgp_msg(c_out, msgs, coefficients, size);

	
	// SECRET KEY GENERATION

	fgp_private_key * key = malloc(sizeof(*key));
	skey_new(key);
	skey_gen(key);
	
	// VERIFICATION PREPARATION
	
	// Generate the verification key
	
	fgp_vkf ** ver_keys = malloc(sizeof(*ver_keys)* size);
	
	for (int i = 0; i <  size; ++i)
	{
		ver_keys[i] = malloc(sizeof(*ver_keys[i]));
		fgp_vkf_new(ver_keys[i]);
		
		fgp_vkf_set(ver_keys[i], key, L[i]);
	}
	
	// and compute over them
	
	fgp_vkf * Wf = malloc(sizeof(*Wf));
	fgp_vkf_new(Wf);
	func_fgp_vkf(Wf, ver_keys, coefficients, size);
	

	// AUTHENTICATION

	fgp_tag ** tags = malloc(sizeof(fgp_tag *)*size);

	for (int i = 0; i < size; ++i)
	{
		tags[i] = malloc(sizeof(*tags[i]));
		fgp_tag_new(tags[i]);

		fgp_authenticate(tags[i], key, delta, L[i], msgs[i]);
	}	

	

	// TAG COMPUTATION

	fgp_tag * sigma = malloc(sizeof(*sigma));
	fgp_tag_new(sigma);

	func_fgp_tag(sigma, tags, coefficients, size);

	// VERIFICATION

	int check = 0;

	fgp_verify(&check, key, delta, Wf, c_out, sigma);

	if (check == 0)
		printf("Verification FAILED\n");
	else
		printf("Verification SUCCESSFUL\n");

	// CLEAN UP

	fgp_close();

	for (int i = size-1; i >= 0; --i)
	{
		fgp_tag_free(tags[i]);
		fgp_vkf_free(ver_keys[i]);
		fgp_msg_free(msgs[i]);
		free(L[i]);
	}
	free(msgs);
	free(tags);
	free(ver_keys);

	fgp_msg_free(c_out);
	fgp_vkf_free(Wf);
	fgp_tag_free(sigma);
	skey_free(key);

	free(L);

	return 0;
}

