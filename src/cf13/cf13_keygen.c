#include "cf13_keygen.h"

int cf13_skey_gen(cf13_private_key *key)
{
	// Take random K
	rand_bytes(key->K, key->size);
	
	// Choose random x
	
	/*
	 * Random generation using FLINT should
	 * be replaced.
	 */
	flint_rand_t state;
	flint_randinit(state);
	
	
	
	fq_randtest(key->x, state, key->ctx);
	
	//fq_print(key->x, key->ctx);
	
	flint_randclear(state);
	
	return 0;
}
