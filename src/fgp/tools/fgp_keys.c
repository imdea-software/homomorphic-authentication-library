#include "fgp_keys.h"

int skey_new(fgp_private_key *key)
{
	// Not an extended key
	key->extended = 0;
	// Allocate the memory
	key->size = KEY_SIZE;
	key->K1 = malloc(sizeof(uint8_t)*KEY_SIZE);
	key->K2 = malloc(sizeof(uint8_t)*KEY_SIZE);
	bn_new_size(&(key->alpha), RELIC_DIGS);
	bn_new_size(&(key->beta), RELIC_DIGS);
	bn_new_size(&(key->zed), RELIC_DIGS);
	return 0;
}

int skey_free(fgp_private_key *key)
{	
	free(key->K1);
	free(key->K2);
	bn_clean(&(key->alpha));
	bn_clean(&(key->beta));
	bn_clean(&(key->zed));
	
	if (key->extended == 1)
	{
		bn_clean(&(key->inv_zed));
	}

	free(key);
	return 0;
}

/*
int pkey_new(public_key * key)
{
	return 0;
}

int pkey_free(public_key * key)
{
	return 0;
}
*/