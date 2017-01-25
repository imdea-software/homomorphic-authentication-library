#ifndef _FGP_PRF_H_
#define _FGP_PRF_H_
#include "fgp_prf.h"
#endif

/*! \file fgp_keys.h
    \brief File describing the key structure.
    
	Handling of the public and private key structures.
	The public key stores relic configuration variables as well as polynomial degrees.
	The private key can be extended to include som precomputation variables.  

    \ingroup tools
*/



/*! \struct public_key
 *  The public key stores relic configuration variables.
 *	This structure is not used as of now.
 */
typedef struct
{
	// Ciphertext degree
	int degree;
	// Relic curve identifier
	int curve;
	// Size of a prime field element
	int fp_bits;
	// Finite field identifier
	int field;
}public_key;


/*! \struct fgp_private_key
 *  The private key stores:
 *	- the 2 prf fgp_keys K1 and K2 of size size in bytes.
 *	- the field members alpha, beta, zed
 *
 *	The key can be extended to include the inverse of zed in the prime field @inv_zed
 *	as well as precomputation tables for the exponentiation of the generators of
 * 	the elliptic groups preComp1 and preComp2.
 */
typedef struct
{
	// The bit to set if extended
	int extended;

	// Basic key

	// Size of the K1 K2 fgp_keys
	int size;
	uint8_t *K1;
	uint8_t *K2;

	bn_st alpha;
	bn_st beta;
	bn_st zed;

	// Extended key

	// Inverse of zed
	bn_st inv_zed;

	// Precomputation tables 
	g1_t *preComp1;	
	g2_t *preComp2;	

}fgp_private_key;

/**
 * Allocates a basic (non-extended) secret key, and sets the size parameter.
 * To extend it, call skey_extend.
 * @param[in] key 	Pointer to the structure to allocate.
 */
int skey_new(fgp_private_key *key);

/**
 * Allocates an secret key. If the key was allocated as a basic key by skey_new,
 * allocate only the non-previously allocated memory.
 * Sets the extended bit to 1.
 * @param[in] key 	Pointer to the structure to allocate.
 */
int skey_extend(fgp_private_key *key);

/**
 * Frees the structure.
 * @param[in] key 	Pointer to the structure to free.
 */
int skey_free(fgp_private_key *key);

/*
int pkey_new(public_key * key, int _degree);

int pkey_free(public_key * key);
*/

