#include "utils.h"

/*! \file cf13_keys.h
    \brief File describing the key structure.
    
	Handling of the public and private key structures.
	The public key stores the prime number.
	

    \ingroup tools
*/

/*! \struct public_key
 *  The public key stores relic configuration variables.
 *	This structure is not used as of now.
 */
typedef struct
{
	//security parameter
	int sec_param;
	//flint context for Fq with prime modulus q
	fq_ctx_t ctx;
	
}cf13_public_key;


/*! \struct cf13_private_key
 *  The private key stores:
 *	- the prf key K of size size in bytes.
 *	- the field member x
 *
 */
typedef struct
{
	//flint context for Fq with prime modulus q
	fq_ctx_t ctx;
	
	// Size in bytes of the K prf key
	int size;
	uint8_t *K;
	
	fq_t x;

}cf13_private_key;

/**
 * Allocates a basic secret key, and sets the size parameter.
 * To extend it, call skey_extend.
 * @param[in] key 	Pointer to the structure to allocate.
 * @param[in] pk an initialized CF13 public key
 */
int cf13_private_key_new(cf13_private_key *key, cf13_public_key *pk);


/**
 * Frees the structure.
 * @param[in] key 	Pointer to the structure to free.
 */
int cf13_private_key_free(cf13_private_key *key);

/*
int pkey_new(public_key * key, int _degree);

int pkey_free(public_key * key);
*/

