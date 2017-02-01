#include <cf13.h>

int main(int argc, char **argv)
{
	// INITIALIZE
	
	cf13_public_key * pk = malloc(sizeof(*pk));
	if (cf13_init(pk, 128))
	{
		printf("Testing FAILED\n");
		printf("Problem initializing the cf13 scheme\n");
		return 1;
	}

	// SET UP THE TESTING VARIABLES

	int size = 100; 				// Number of coefficients
	
	// CREATE A SECRET KEY

	cf13_private_key * sk = malloc(sizeof(*sk));
	cf13_private_key_new(sk, pk);
	cf13_skey_gen(sk);


	// COMPUTE THE VERIFICATION KEYS
	// And a unique string identifier for each ciphertext within this data set: L[i]

	cf13_vkf ** ver_keys = malloc(sizeof(*ver_keys)* size);
	char ** L = malloc(sizeof(char*)*size);

	for (int i = 0; i <  size; ++i)
	{
		L[i] = malloc(sizeof(char)*MAX_STR_SIZE);
		label(L[i], i);		// Creates a string containing the number i

		ver_keys[i] = malloc(sizeof(*ver_keys[i]));
		cf13_vkf_new(ver_keys[i], sk);
		
		cf13_vkf_set(ver_keys[i], sk, L[i]);
		
	}

	


	// VERIFICATION PREPARATION
	// Done ahead of time, without knowing the inputs
	// For the CF13 scheme, it is not re-usable

	cf13_vkf * Wf = malloc(sizeof(*Wf));
	cf13_vkf_new(Wf, sk);

	cf13_vkf * vkf_tmp = malloc(sizeof(*vkf_tmp));
	cf13_vkf_new(vkf_tmp, sk);

	for (int i = 0; i < size; ++i)
	{
		cf13_vkf_mul(vkf_tmp, ver_keys[i], ver_keys[i]);
		cf13_vkf_add(Wf, Wf, vkf_tmp);
	}
	
	// Create some inputs
	// And compute the function on them

	cf13_tag ** tags = malloc(sizeof(cf13_tag *)*size);
	cf13_tag * res_tag = malloc(sizeof(*res_tag));

	cf13_msg ** msgs = malloc(sizeof(cf13_msg *)*size);
	cf13_msg * m_out = malloc(sizeof(*m_out));
	
	for (int i = 0; i < size; ++i)
	{
		//create a random message
		msgs[i] = malloc(sizeof(*msgs[i]));
		cf13_msg_new(msgs[i], pk);
		cf13_msg_rand(msgs[i]);
		
		//cf13_msg_print(msgs[i]);
		//printf("\n");
		
		//authenticate the message
		tags[i] = malloc(sizeof(*tags[i]));
		cf13_tag_new(tags[i], sk);
		cf13_authenticate(tags[i], sk, L[i], msgs[i]);
		
	}
	
	//computation \sum_i m_i^2
	cf13_msg_new(m_out, pk);
	cf13_tag_new(res_tag, sk);
	
	cf13_tag * tag_tmp = malloc(sizeof(*tag_tmp));
	cf13_tag_new(tag_tmp, sk);
	cf13_msg * msg_tmp = malloc(sizeof(*msg_tmp));
	cf13_msg_new(msg_tmp, pk);
	
	for (int i = 0; i < size; ++i)
	{
		cf13_msg_mul(msg_tmp, msgs[i], msgs[i]);
		cf13_msg_add(m_out, m_out, msg_tmp);
		
		cf13_tag_mul(tag_tmp, tags[i], tags[i]);
		cf13_tag_add(res_tag, res_tag, tag_tmp);
		
	}
	printf("Computation's output: ");
	cf13_msg_print(m_out);
	printf("\n");
	
	//to make the computation on messages incorrect
	//uncomment the following lines
	//cf13_msg_add(m_out, m_out, m_out);
	//printf("Biased computation's output: ");
	//cf13_msg_print(m_out);
	//printf("\n");
	

	int check;
	
	check=0;

	cf13_verify(&check, sk, Wf, m_out, res_tag);

	if (check == 0)
		printf("Verification FAILED\n");
	else
		printf("Verification SUCCESSFUL\n");
	

	// CLEAN UP

	//free results variables
	cf13_tag_free(res_tag);
	cf13_msg_free(m_out);
	cf13_vkf_free(Wf);
	
	//free temp variables
	cf13_tag_free(tag_tmp);
	cf13_msg_free(msg_tmp);
	cf13_vkf_free(vkf_tmp);
	
	//free loop variables
	for (int i = size-1; i >= 0; --i)
	{
		cf13_tag_free(tags[i]);
		cf13_vkf_free(ver_keys[i]);
		cf13_msg_free(msgs[i]);
		free(L[i]);
	}
	//free vectors
	free(msgs);
	free(tags);
	free(ver_keys);
	free(L);

	//free keys
	cf13_private_key_free(sk);
	cf13_close(pk);

	return 0;
}

