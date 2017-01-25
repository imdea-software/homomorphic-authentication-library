#include "fgp_authenticate.h"
#include "fgp_keygen.h"
#include "fgp_verify.h"

/**
 *	\defgroup fgp FGP
 */


/*! \file fgp.h
    \brief File descibing initialization and closing of the fgp context.
     
	The files is the parent of the three fgp files describing the algorithm functions.

    \ingroup fgp
*/


/**
 * Initializes the relic context (sets) the elliptic curve parameter according
 * to the FP_PRIME parameter in the configuration files. The configuration
 * (choice of security paramter for instance) has to be configured before compilation
 * of the library.
 */
int fgp_init();


/**
 * Closes the relic context.
 */
int fgp_close();
