#include "fgp_keygen.h"

int skey_gen(fgp_private_key *key)
{
	// Randomize K1 and K2
	bn_t _k1, _k2;
	bn_new_size(_k1, key->size / RELIC_BN_BYTES);
	bn_new_size(_k2, key->size / RELIC_BN_BYTES);
	bn_rand(_k1, 0, key->size * 8);
	bn_rand(_k2, 0, key->size * 8);

	bn_write_bin(key->K1, key->size, _k1);
	bn_write_bin(key->K2, key->size, _k2);

	bn_clean(_k1);
	bn_clean(_k2);

	// Randomize the rest
	bn_t mod;
	bn_new_size(mod, RELIC_DIGS);
	g1_get_ord(mod);

	bn_rand_mod(&(key->alpha), mod);
	bn_rand_mod(&(key->beta), mod);
	bn_rand_mod(&(key->zed), mod);

	bn_clean(mod);
	return 0;
}
