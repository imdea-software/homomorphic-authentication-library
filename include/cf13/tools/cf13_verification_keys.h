#include "utils.h"

#ifndef _cf13_keys_H_
#define _cf13_keys_H_
#include "cf13_keys.h"
#endif

#ifndef _cf13_msg_H_
#define _cf13_msg_H_
#include "cf13_msg.h"
#endif

#ifndef _PRF_H_
#define _PRF_H_
#include "prf.h"
#endif

/*! \file cf13_verification_keys.h
    \brief File describing the verification cf13_keys.
    
	Verification cf13_keys are elements of Z/qZ.

    \ingroup tools
*/

/*! \struct cf13_vkf
 * 	A basic verification key is created with the value \f$ r \f$ output of the prf.
 */
typedef struct
{
	fq_ctx_t ctx;
	
	fq_t r;
	
}cf13_vkf;

/**
 * Allocate memory for the value of the key and set the context.
 * @param[in] r 		Pointer to the cf13_vkf to allocate.
 */
int cf13_vkf_new(cf13_vkf * r, cf13_private_key *sk);

/**
 * Deallocate memory for the value and free the strucuture pointer.
 * @param[in] r 		Pointer to the cf13_vkf to deallocate.
 */
int cf13_vkf_free(cf13_vkf *r);


/**
 * Copy one cf13_vkf into antoher.
 * @param[out] r_out 		Key to copy into.
 * @param[in] r_in 			Key to copy from.
 */
int cf13_vkf_copy(cf13_vkf *r_out, cf13_vkf *r_in);

/**
 * Add two cf13_vkf together.
 * @param[out] r_out 		Output key.
 * @param[in] r1 			First input.
 * @param[in] r2 			Second input.
 */
int cf13_vkf_add(cf13_vkf * r_out, cf13_vkf * r1, cf13_vkf * r2);

/**
 * Multiply two cf13_vkf together (polynomial multiplication).
 * @param[out] r_out 		Output key.
 * @param[in] r1 			First input.
 * @param[in] r2 			Second input.
 */
int cf13_vkf_mul(cf13_vkf *r_out, cf13_vkf *r1, cf13_vkf *r2);

/**
 * Multiply a cf13_vkf by a constant.
 * @param[out] r_out 		Output key.
 * @param[in] r_in 			Input key.
 * @param[in] constant 		Constant to multiply by.
 */
int cf13_vkf_const_mult(cf13_vkf * r_out, cf13_vkf * r_in, cf13_msg * constant);

/**
 * Add a constant to a key.
 * @param[out] r_out 		Output key.
 * @param[in] r_in 			Input key.
 * @param[in] constant 		Constant to add.
 * @param[in] key 			Secret key.
 */
int cf13_vkf_const_add(cf13_vkf * r_out, cf13_vkf * r_in, cf13_msg * constant);


/**
 * Set a verification key corresponding to a label represented by
 * the string L, using the pseudo random function initialized with the key key.
 * @param[out] r 			The initialized verification key.
 * @param[in] key 			The key.
 * @param[in] L 			The label that goes through the prf.
 */
int cf13_vkf_set(cf13_vkf * r, cf13_private_key * key, char *  L);


/**
 * Print the cf13_vkf specifications (degree) and variables to
 * standard output.
 * @param[in] r 	Pointer to the cf13_vkf to print.
 */
int cf13_vkf_print(cf13_vkf *r);

/**
 * Write any cf13_vkf to file
 * @param[out] file 	File to write to.
 * @param[in] r 		Pointer to the cf13_vkf to write.
 */
int cf13_vkf_write(unsigned char * file, cf13_vkf * r);

/**
 * Read any cf13_vkf from file
 * @param[out] s 		Pointer to the cf13_vkf to read into.
 * @param[int] file 	File to read from.
 */
int cf13_vkf_read(cf13_vkf * r, unsigned char * file);

