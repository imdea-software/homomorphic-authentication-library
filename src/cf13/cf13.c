#include "cf13.h"

int cf13_init(cf13_public_key *pk, int sec_param)
{
	// Set up the RELIC context
	// Used only for RNG
	if (core_init() != STS_OK)
	{
		core_clean();
		return 1;
	}
	
	
	pk->sec_param = sec_param;
	
	fmpz_t q;
	fmpz_init(q);
	
	do{
		//generate random integer sec_param bits long
		if(hal_get_randlen_fmpz(q, sec_param))
			return 1;
		//primality test
	}
	while(!fmpz_is_probabprime(q));
	//initialize fq context with prime q
	fq_ctx_init(pk->ctx, q, 1, "ctx");
	
	return 0;
}

int cf13_close(cf13_public_key *pk)
{
	core_clean();
	
	fq_ctx_clear(pk->ctx);
	
	free(pk);
	
	return 0;
}
