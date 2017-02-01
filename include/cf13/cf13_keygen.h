#ifndef _CF_TOOLS_H_
#define _CF_TOOLS_H_
#include "cf13_tools.h"
#endif

/*! \file cf13_tools.h
    \brief File describing key generation function
     
	The function generates a random key into a previously allocated structure.

    \ingroup cf13
*/

/**
 * Initializes the alpha beta zed members of Zq and the two prf keys
 * The relic context HAS to be initialized before hand
 * @param[out] key 		Structure to fill with the random elements created.
 */
int cf13_skey_gen(cf13_private_key *key);
