#include "cf13_keygen.h"

int cf13_skey_gen(cf13_private_key *key)
{
	// Take random K
	if(hal_get_rand_bytes(key->K, key->size))
		return 1;
	
	// Choose random x
	
	if(hal_get_rand_fq(key->x, key->ctx))
		return 1;
	
	return 0;
}
