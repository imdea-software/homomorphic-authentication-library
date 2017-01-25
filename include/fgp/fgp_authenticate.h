#ifndef _TOOLS_H_
#define _TOOLS_H_
#include "fgp_tools.h"
#endif

/*! \file fgp_authenticate.h
    \brief File describing the authenticate function
     
	The function uses a secret key to authenticate a polynomial with a unique multi-label
	consisting of two strings (delta, L).

    \ingroup fgp
*/


/**
 * The authentication is made by creating a degree 1 tag from the input
 * message polynomial (more precisely its evaluation in \f$(\beta, \alpha)\f$ from the key).
 * @param[out] s 		Tag created.
 * @param[in] key 		Secret key used.
 * @param[in] delta		The unique data set identifier.
 * @param[in] L 		Unique input identifier within the data set.
 * @param[in] pol 		The polynomial to authenticate.
 */
int fgp_authenticate(fgp_tag *s, fgp_private_key * key, char * delta, char * L, fgp_msg * pol);



