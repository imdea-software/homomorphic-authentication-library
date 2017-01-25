#ifndef _TOOLS_H_
#define _TOOLS_H_
#include "fgp_tools.h"
#endif

/*! \file fgp_tools.h
    \brief File describing key generation function
     
	The function generates a random key into a previously allocated structure.

    \ingroup fgp
*/

/**
 * Initializes the alpha beta zed members of Zq and the two prf keys
 * The relic context HAS to be initialized before hand
 * @param[out] key 		Structure to fill with the random elements created.
 */
int skey_gen(fgp_private_key *key);