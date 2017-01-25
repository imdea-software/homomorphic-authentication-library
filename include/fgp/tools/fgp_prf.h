#ifndef _UTILS_H_
#define _UTILS_H_
#include "utils.h"
#endif

/*! \file fgp_prf.h
    \brief File describing the pseudo random function and sub-functions it uses
    
	The prf is created through an hmac using different hashes depending on the macro HASH_LEN
	and the result is expanded to size EXP_LEN and then reduced to the appropriate output size.

    \ingroup tools
*/


/*! \def SEC_PARAM
    \brief The security parameter of the protocol.
   
	Is set from the relic configuration so that 2*SEC_PARAM is
	the size of the order
*/
#ifndef SEC_PARAM
#define SEC_PARAM (FP_BITS/2)
#endif


/*! \def KEY_SIZE
    \brief The size of the two keys \f$ \alpha \f$ and \f$ \beta \f$
   
	Is set to be the security parameter.
*/
#ifndef KEY_SIZE
#define KEY_SIZE SEC_PARAM
#endif


/*! \def EXP_LEN
    \brief The size to which to expand the encrytion result before reduction for the prf
   
	The value SEC_PARAM + FP_BITS always verifies the equations described in the expand"()" function.
*/
#ifndef EXP_LEN
#define EXP_LEN (int)(SEC_PARAM + FP_BITS)
#endif

/**
 * From the input message and the key, output two integers
 * \f$ (a,b) \in [1,q]^{2} \f$ in a pseudo random fashion, where q is the order
 * of the elliptic curve groups.
 * @param[out] a 			First output.
 * @param[out] b 			Second output.
 * @param[in] input 	 	Input message as a string of characters terminated by '\0'
 * @param[in] key 	 		The key as byte vector.
 * @param[in] key_size 	 	The size of the key.
 */
int decouple(bn_st * a, bn_st * b, char * input, uint8_t * key, int key_size);