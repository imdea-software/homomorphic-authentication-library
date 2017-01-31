#ifndef _CF_TOOLS_H_
#define _CF_TOOLS_H_
#include "cf13_tools.h"
#endif

/*! \file cf13_verify.h
    \brief File describing the verifiy function
     
	The verify function checks that the computation that was made on the
	messages was correct.

    \ingroup cf13
*/


/**
 * If the verification equation is satisfied set the check bit to \f$ 0 \f$ (accept), otherwise to \f$ 1 \f$ (reject).
 * @param[out] check 	Pointer to the structure to allocate.
 * @param[in] key 		Secret key used for the authentication and v-key computation.
 * @param[in] ver_key 	Verification key computed previously.
 * @param[in] m 		The message output of the computation to check.
 * @param[in] s 		The tag computed by applying the computation to the authentication tags.
 */
int cf13_verify(int * check, cf13_private_key * key, cf13_vkf * ver_key, cf13_msg * m, cf13_tag * s);
