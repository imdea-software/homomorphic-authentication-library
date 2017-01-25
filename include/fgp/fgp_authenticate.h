#ifndef _TOOLS_H_
#define _TOOLS_H_
#include "fgp_tools.h"
#endif

/*! \file fgp_authenticate.h
    \brief File describing the authenticate function
     
	The function authenticates a ciphertext with a tag from a key and a unique label.

    \ingroup fgp
*/


/**
 * The authentication is made by creating a degree 1 tag from the input
 * ciphertext (more precisely its evaluation in \f$(\beta, \alpha)\f$ from the key).
 * @param[out] s 		Tag created.
 * @param[in] key 		Secret key used.
 * @param[in] delta		The unique data set identifier.
 * @param[in] L 		Unique ciphertext identifier within the data set.
 * @param[in] pol 		The ciphertext to authenticate.
 */
int fgp_authenticate(fgp_tag *s, fgp_private_key * key, char * delta, char * L, fgp_msg * pol);



