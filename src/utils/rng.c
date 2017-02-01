#include "rng.h"

int hal_get_rand_bytes(uint8_t * v, int size)
{
	// get random bytes using the RELIC RNG
	rand_bytes(v, size);
	
	return 0;
}

int hal_get_rand_fq(fq_t r, fq_ctx_t ctx)
{
	fmpz * q;
	q = fq_ctx_prime(ctx);
	mp_bitcnt_t q_len = fmpz_bits(q);
	int size = q_len;
	fmpz_t temp;
	fmpz_init(temp);
	do {
		//generate a random integer in [0, 2^q_len -1]
		hal_get_randbits_fmpz(temp, size);
		//until it is < q
	} while (fmpz_cmp(temp, q)>0);
	
	//convert to fq set the output
	fq_set_fmpz(r, temp, ctx);
	
	return 0;
}

int hal_get_randbits_fmpz(fmpz_t r, int bits)
{
	int byte_number = bits/8 + 1*(bits%8 != 0);
	
	uint8_t * v = malloc(sizeof(uint8_t)*byte_number);
	
	// first get random bytes
	hal_get_rand_bytes(v, byte_number);
	// if bits is not a multiple of 8
	// force the most significant bits of v to be 0
	// so that it represents an integer < 2^size
	// This is done by setting to 0 all the last (8-rm)
	// bits of the first byte
	// This trick in particular improves rejection sampling
	// as used in hal_get_rand_fq so that on average 2
    // trials are sufficient
	int rm = bits%8;
	if(rm>0)
	{
		for(int j=rm; j<8; ++j)
			v[0] &= ~(1 << j);
	}
	//convert from uint8_t into an integer
	if(uint8_t_to_fmpz_t(r, v, byte_number))
		return 1;
	
	return 0;
}

int hal_get_randlen_fmpz(fmpz_t r, int bits)
{
	int byte_number = bits/8 + 1*(bits%8 != 0);
	
	uint8_t * v = malloc(sizeof(uint8_t)*byte_number);
	
	// first get random bytes
	hal_get_rand_bytes(v, byte_number);
	// then manipulate the bits of the byte string
	// so that it is an integer of precisely bits bits
	
	// if bits is not a multiple of 8
	// force the (8-rm) most significant bits of v to be 0
	// and the 8th MSB to be 1
	// so that it represents an integer in [2^{bits-1},2^bits -1]
	int rm = bits%8;
	if(rm>0)
	{
		v[0] |= 1;
		for(int j=rm; j<8; ++j)
			v[0] &= ~(1 << j);
	}
	else // if bits is a multiple of 8 we simply set the MSB to 1
		v[0] |= (1<<7);
	//convert from uint8_t into an integer
	if(uint8_t_to_fmpz_t(r, v, byte_number))
		return 1;
	
	return 0;
}
