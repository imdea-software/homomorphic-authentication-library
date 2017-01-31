#include "cf13_keys.h"

int cf13_private_key_new(cf13_private_key *key, cf13_public_key *pk)
{
	// Allocate the memory
	key->size = pk->sec_param / 8;
	key->K = malloc(sizeof(uint8_t)*key->size);
	fq_ctx_init(key->ctx, fq_ctx_prime(pk->ctx), fq_ctx_degree(pk->ctx), "ctx");
	fq_init(key->x, key->ctx);
	
	return 0;
}

cf13_private_key_free(cf13_private_key *key)
{	
	free(key->K);
	fq_clear(key->x, key->ctx);
	
	fq_ctx_clear(key->ctx);
	
	free(key);
	return 0;
}
