#include <fgp.h>

int main(int argc, char **argv)
{
	/**
	
	The computation that is performed on the messages in this example is as follows:

	(c_0·a_0 + .. c_n·a_n) + (c_0·c_1 + .. + c_{n-1}·c_n)

	where c_i are the message polynomials and a_i some integer coefficients.

	**/

	// INITIALIZE THE FGP SCHEME
	
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
	
	// Create some random coefficients to define the function

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


	// KEY GENERATION

	fgp_private_key * key = malloc(sizeof(*key));
	skey_new(key);
	skey_gen(key);


	// COMPUTE THE VERIFICATION KEYS
	// And a unique label for each input i: L[i]

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
	// Done ahead of time, without knowing the computations' inputs

	fgp_vkf * Wf = malloc(sizeof(*Wf));
	fgp_vkf_new(Wf);

	fgp_vkf * vkf_tmp = malloc(sizeof(*vkf_tmp));
	fgp_vkf_new(vkf_tmp);

	for (int i = 0; i < size; ++i)
	{
		fgp_vkf_const_mult(vkf_tmp, ver_keys[i], coefficients[i]);
		fgp_vkf_add(Wf, Wf, vkf_tmp);
	}

	// INPUT GENERATION
	// This is a matrix of messages; the function will be applied to every row
	
	// Allocate and initialize messages
	fgp_msg *** msgs = malloc(sizeof(fgp_msg **)*datasets);
	for (int i = 0; i < datasets; ++i)
	{
		msgs[i] = malloc(sizeof(fgp_msg*)*size);
		for (int j = 0; j < size; ++j)
		{
			msgs[i][j] = malloc(sizeof(*msgs[i][j]));
			fgp_msg_new(msgs[i][j]);
		}
	}
	// vector of computation's outputs
	fgp_msg ** m_out = malloc(sizeof(fgp_msg *)*datasets);

	// Allocate and initialize tags for input messages
	fgp_tag ** tags = malloc(sizeof(fgp_tag *)*size);
	for (int i = 0; i < size; ++i)
	{
		tags[i] = malloc(sizeof(*tags[i]));
		fgp_tag_new(tags[i]);
		
	}
	// vector of computation's outputs tags
	fgp_tag ** sigma = malloc(sizeof(fgp_tag *)*datasets);
	
	
	// Temporary variables for the computation

	fgp_tag * tag_tmp = malloc(sizeof(*tag_tmp));
	fgp_tag_new(tag_tmp);

	fgp_msg * poly_tmp = malloc(sizeof(*poly_tmp));
	fgp_msg_new(poly_tmp);

	
	int check;

	// COMPUTATION
	
	// Create some messages (as polynomials of degree 1 or less in Y)
	// And compute the function on them

	for (int j = 0; j < datasets; ++j)
	{
		for (int i = 0; i < size; ++i)
		{
			// generate random message
			fgp_msg_rand(msgs[j][i], 100, 1);
			// Authenticate
			fgp_authenticate(tags[i], key, delta[j], L[i], msgs[j][i]);
		}

		// Computation on messages

		m_out[j] = malloc(sizeof(*m_out[j]));
		fgp_msg_new(m_out[j]);

		for (int i = 0; i < size; ++i)
		{
			fgp_msg_const_mult(poly_tmp, msgs[j][i], coefficients[i]);
			fgp_msg_add(m_out[j], m_out[j], poly_tmp);
		}

		// Computation on tags

		sigma[j] = malloc(sizeof(*sigma[j]));
		fgp_tag_new(sigma[j]);

		fgp_tag_const_mult(sigma[j], tags[0], coefficients[0]);
		
		for (int i = 1; i < size; ++i)
		{
			fgp_tag_const_mult(tag_tmp, tags[i], coefficients[i]);
			fgp_tag_add(sigma[j], sigma[j], tag_tmp);
		}

		// VERIFICATION OF THE j-th FUNCTION
		check = 0;

		printf("Dataset %s: ", delta[j]);
		fgp_verify(&check, key, delta[j], Wf, m_out[j], sigma[j]);

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
		fgp_msg_free(m_out[i]);
		fgp_tag_free(sigma[i]);
	}
	fgp_tag_free(tag_tmp);
	fgp_msg_free(poly_tmp);
	free(sigma);
	free(m_out);
	free(delta);

	for (int i = size-1; i >= 0; --i)
	{
		fgp_tag_free(tags[i]);
		fgp_vkf_free(ver_keys[i]);
		for(int j= datasets-1; j>=0; --j)
			fgp_msg_free(msgs[j][i]);
		free(L[i]);
	}
	free(msgs);
	free(tags);
	free(ver_keys);
	free(L);

	fgp_vkf_free(Wf);
	skey_free(key);

	fgp_vkf_free(vkf_tmp);

	return 0;
}

