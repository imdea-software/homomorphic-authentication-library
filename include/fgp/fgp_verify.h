#ifndef _TOOLS_H_
#define _TOOLS_H_
#include "fgp_tools.h"
#endif

/*! \file fgp_verify.h
    \brief File describing the verify function
     
	The verify function checks that the computation that was made on the
	polynomials was correct.

    \ingroup fgp
*/


/**
 * Verifies that the computation done to obtain the polynomial pol was correct. This is
 * done by checking the verification equations.
 * The verification function applies the following computations: \n
 * Compute \f$ (a, b) \leftarrow F'_{K_2}(\delta) \f$ and \f$ W = e(g,h)^{\omega_f(a,b)} \f$ \n
 * Compute\f$ \nu = \mu(\beta, \alpha) \f$ \n
 * If \f$ \deg(f)=2 \f$, check the following equalities over \f$ G_T \f$:
 * \f[
 * \begin{array}{ccc}
 * 	T & = & U = e(g,h)^{\nu} \label{eq:ver1} \\
 *	X & = & Y   \label{eq:ver2} \\
 *	W & = & T \cdot (X)^{z} \cdot (\Lambda)^{z^{2}} 
 * \end{array}
 * \f]
 * If \f$ \deg(f)=1 \f$,  check the following equations:
 * \f[
 * \begin{array}{ccc}
 * 	T & = & g^{\nu} \\
 * 	U & = & h^{\nu} \\
 *	e(X, h) & = & e(g, Y)    \\
 *	W & = & e(T \cdot (X)^{z}, h)
 * \end{array}
 * \f]
 * If all equations are satisfied set the check bit to \f$ 1 \f$ (accept), otherwise to \f$ 0 \f$ (reject).
 * @param[out] check 	Pointer to the structure to allocate.
 * @param[in] key 		Secret key used for the authentication and vkf computation.
 * @param[in] delta		The unique string representing the data set on which the computation was made.
 * @param[in] ver_key 	Verification key computed previously as verification preparation.
 * @param[in] pol 		The polynomial output of the computation to check.
 * @param[in] s 		The tag computed by applying the computation to the authentication tags.
 */
int fgp_verify(int * check, fgp_private_key * key, char * delta, fgp_vkf * ver_key, fgp_msg * pol, fgp_tag * s);
