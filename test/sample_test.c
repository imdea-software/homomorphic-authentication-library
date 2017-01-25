#include <fgp.h>

int main(int argc, char **argv)
{
	/**
	
	The computation that is performed on the cipherexts in this example is as follows:

	(c_0·a_0 + .. c_n·a_n) + (c_0·c_1 + .. + c_{n-1}·c_n)

	where c_i are the ciphertexts and a_i some coefficients.

	**/

	// INITIALIZE
	
	if (fgp_init())
	{
		printf("Testing FAILED\n");
		printf("Problem initializing the library\n");
		return 1;
	}

	// SET UP THE TESTING VARIABLES

	int size = 100; 				// Number of coefficients
	int datasets = 30;			// Number of different datasets


	// Initialize the dataset identifiers
	char ** delta = malloc(sizeof(char*)*datasets);

	for (int i = 0; i < datasets; ++i)
	{
		delta[i] = malloc(sizeof(char)*MAX_STR_SIZE);
		label(delta[i], i);				// Creates a string containing the number i
	}
	
	// Create some coefficients to use in a function

	// Get the order of the elliptic curve groups

	bn_t order;
	bn_new(order);
	g1_get_ord(order);

	bn_t * coefficients = malloc(sizeof(bn_t)*size);
	for (int i = 0; i < size; ++i)
	{
		bn_new(coefficients[i]);
		bn_rand_mod(coefficients[i], order);
	}


	// CREATE A RANDOM SECRET KEY

	fgp_private_key * key = malloc(sizeof(*key));
	skey_new(key);
	skey_gen(key);


	// COMPUTE THE VERIFICATION KEYS
	// And a unique string identifier for each ciphertext within this data set: L[i]

	fgp_vkf ** ver_keys = malloc(sizeof(*ver_keys)* size);
	char ** L = malloc(sizeof(char*)*size);

	for (int i = 0; i <  size; ++i)
	{
		L[i] = malloc(sizeof(char)*MAX_STR_SIZE);
		label(L[i], i);		// Creates a string containing the number i

		ver_keys[i] = malloc(sizeof(*ver_keys[i]));
		fgp_vkf_new(ver_keys[i]);
		
		fgp_vkf_set(ver_keys[i], key, L[i]);
	}



	// VERIFICATION PREPARATION
	// Done ahead of time, without knowing the ciphertexts

	fgp_vkf * Wf = malloc(sizeof(*Wf));
	fgp_vkf_new(Wf);

	fgp_vkf * vkf_tmp = malloc(sizeof(*vkf_tmp));
	fgp_vkf_new(vkf_tmp);

	for (int i = 0; i < size; ++i)
	{
		fgp_vkf_const_mult(vkf_tmp, ver_keys[i], coefficients[i]);
		fgp_vkf_add(Wf, Wf, vkf_tmp);
	}


	// Create some ciphertexts (polynomials of degree 1 or less in Y)
	// And compute the function on them

	fgp_tag ** tags = malloc(sizeof(fgp_tag *)*size);
	fgp_tag ** sigma = malloc(sizeof(fgp_tag *)*datasets);

	fgp_msg *** ciphers = malloc(sizeof(fgp_msg **)*datasets);
	for (int i = 0; i < datasets; ++i)
	{
		ciphers[i] = malloc(sizeof(fgp_msg*)*size);
		for (int j = 0; j < size; ++j)
		{
			ciphers[i][j] = malloc(sizeof(*ciphers[i][j]));
			fgp_msg_new(ciphers[i][j]);
		}
	}
	fgp_msg ** c_out = malloc(sizeof(fgp_msg *)*datasets);

	// Temporary variables for the computation

	fgp_tag * tag_tmp = malloc(sizeof(*tag_tmp));
	fgp_tag_new(tag_tmp);

	fgp_msg * poly_tmp = malloc(sizeof(*poly_tmp));
	fgp_msg_new(poly_tmp);

	for (int i = 0; i < size; ++i)
	{
		tags[i] = malloc(sizeof(*tags[i]));
		fgp_tag_new(tags[i]);

		//ciphers[i] = malloc(sizeof(*ciphers[i]));
		//fgp_msg_new(ciphers[i]);
	}

	int check;

	for (int j = 0; j < datasets; ++j)
	{
		for (int i = 0; i < size; ++i)
		{
			// Creation of ciphertexts
			fgp_msg_rand(ciphers[j][i], 100, 1);
			// Authentication
			fgp_authenticate(tags[i], key, delta[j], L[i], ciphers[j][i]);
		}

		// Ciphertext computation

		c_out[j] = malloc(sizeof(*c_out[j]));
		fgp_msg_new(c_out[j]);

		//fgp_msg_const_mult(c_out[j], ciphers[j][0], coefficients[0]);
		
		for (int i = 0; i < size; ++i)
		{
			fgp_msg_const_mult(poly_tmp, ciphers[j][i], coefficients[i]);
			fgp_msg_add(c_out[j], c_out[j], poly_tmp);
		}

		// Tag computation

		sigma[j] = malloc(sizeof(*sigma[j]));
		fgp_tag_new(sigma[j]);

		fgp_tag_const_mult(sigma[j], tags[0], coefficients[0]);
		
		for (int i = 1; i < size; ++i)
		{
			fgp_tag_const_mult(tag_tmp, tags[i], coefficients[i]);
			fgp_tag_add(sigma[j], sigma[j], tag_tmp);
		}

		// Verification
		check = 0;

		fgp_verify(&check, key, delta[j], Wf, c_out[j], sigma[j]);

		if (check == 0)
			printf("Verification FAILED\n");
		else
			printf("Verification SUCCESSFUL\n");
	}

	// CLEAN UP

	fgp_close();

	for (int i = datasets-1; i >= 0; --i)
	{
		free(delta[i]);
		fgp_msg_free(c_out[i]);
		fgp_tag_free(sigma[i]);
		//fgp_tag_free(tag_tmp[i]);
		//fgp_msg_free(poly_tmp[i]);
	}
	fgp_tag_free(tag_tmp);
	fgp_msg_free(poly_tmp);
	free(sigma);
	free(c_out);
	free(delta);

	for (int i = size-1; i >= 0; --i)
	{
		fgp_tag_free(tags[i]);
		fgp_vkf_free(ver_keys[i]);
		//fgp_msg_free(ciphers[i]);
		free(L[i]);
	}
	//free(ciphers);
	free(tags);
	free(ver_keys);
	free(L);

	fgp_vkf_free(Wf);
	skey_free(key);

	fgp_vkf_free(vkf_tmp);

	return 0;
}

