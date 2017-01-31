#include "cf13_authenticate.h"
#include "cf13_keygen.h"
#include "cf13_verify.h"

/**
 *	\defgroup cf13 CF13
 */


/*! \file cf13.h
    \brief File descibing initialization and closing of the cf13 context.
     
	The files is the parent of the three cf13 files describing the algorithm functions.

    \ingroup cf13
*/


/**
 * Initializes a flint context, choosing a random prime modulus of sec_param bits.
 */
int cf13_init(cf13_public_key *pk, int sec_param);

/**
 * Clears a flint context.
 */
int cf13_close(cf13_public_key *pk);
