#ifndef _FGP_PRF_H_
#define _FGP_PRF_H_
#include "fgp_prf.h"
#endif

/*! \file fgp_msg.h
    \brief File describing the polynomial structure.
    
    The main purpose of the functions on polynomials (appart from allocation etc..)
    is to evaluate them for the authentication and verification processes as the ciphertexts
    are in effect reduced to their evaluation in (beta, alpha) from the key.

    \ingroup tools
*/

#ifndef DEGREE_PARAM
#define DEGREE_PARAM 5000 // n
#endif

#ifndef MAX_DEGREE
#define MAX_DEGREE 2*(DEGREE_PARAM - 1) + 6
#endif

/*! \struct fgp_msg
 * Polynomial strucure based on the FLINT library. The fgp_msg structure
 * contains three polynomials and a prime field context that contains the
 * order of the elliptic curve groups.
 * The resulting bivariate polynomial is as follows:
 * \f[
 * 	xpol + xypol \cdot Y + xy2pol \cdot Y^{2}
 * \f]
 */
typedef struct
{
	fq_ctx_t ctx;

	fq_poly_t xpol;
	fq_poly_t xypol;
	fq_poly_t xy2pol;

} fgp_msg;


/**
 * Polynomial allocation. Allocates an empty polynomial. The polynomial created is set to 0.
 * @param[in] pol 		Pointer to a polynomial to allocate.
 */
int fgp_msg_new(fgp_msg * pol);

/**
 * Evaluates the polynomial.
 * @param[out] output 	Pointer to the result of the evaluation in the relic prime field.
 * @param[in] pol 		Pointer to a polynomial to evaluate.
 * @param[in] beta		X variable with which to evaluate the polynomial.
 * @param[in] alpha 	Y variable with which to evaluate the polynomial.
 */
int fgp_msg_eval(bn_st * output, fgp_msg * pol, bn_st * beta, bn_st * alpha);

/**
 * Frees the structure entirely.
 * @param[in] pol 		Pointer to the structure to free.
 */
int fgp_msg_free(fgp_msg * pol);

/**
 * Write a polynomial to file
 * @param[out] file_name 	File to write to.
 * @param[in] pol 			Pointer to the polynomial to write.
 */
int fgp_msg_write(unsigned char * file_name, fgp_msg * pol);

/**
 * Read a polynomial from file
 * @param[out] pol			Pointer to the polynomial to read into.
 * @param[int] file_name 	File to read from.
 */
int fgp_msg_read(fgp_msg * pol, unsigned char * file_name);

/**
 * Create a random polynomial of degree xdegree in x and ydegree in y.
 * @param[out] pol 			Pointer to the polynomial to set.
 * @param[in] xdegree 		X degree to set.
 * @param[in] yydegree		Y degree to set.
 */
int fgp_msg_rand(fgp_msg * pol, int xdegree, int ydegree);

/**
 * Add two polynomials together
 * @param[out] pol_out 		Pointer to the output polynomial.
 * @param[in] pol1			First input.
 * @param[in] pol2			Second input.
 */
int fgp_msg_add(fgp_msg * pol_out, fgp_msg * pol1, fgp_msg * pol2);

/**
 * Copy a polynomial into another.
 * @param[out] pol_out 		Pointer to the output polynomial.
 * @param[in] pol_in		Polyomial to copy from.
 */
int fgp_msg_copy(fgp_msg * pol_out, fgp_msg * pol_in);

/**
 * Multiply two polynomials together
 * @param[out] pol_out 		Pointer to the output polynomial.
 * @param[in] pol1			First input.
 * @param[in] pol2			Second input.
 */
 int fgp_msg_mul(fgp_msg * pol_out, fgp_msg * pol1, fgp_msg * pol2);

 /**
 * Multiplies a polynomial by a constant.
 * @param[out] poly_out			Output polynomial.
 * @param[in] poly_in			Input polynomial.
 * @param[in] constant			The constant by which to multiply the polynomial.
 */
int fgp_msg_const_mult(fgp_msg * pol_out, fgp_msg * pol_in, bn_st * constant);

 /**
 * Adds a constant to a polynomial.
 * @param[out] poly_out			Output polynomial.
 * @param[in] poly_in			Input polynomial.
 * @param[in] constant			The constant to add to the the polynomial.
 */
int fgp_msg_const_add(fgp_msg * pol_out, fgp_msg * pol_in, bn_st * constant);

 /**
 * Substracts a constant to a polynomial.
 * @param[out] poly_out			Output polynomial.
 * @param[in] poly_in			Input polynomial.
 * @param[in] constant			The constant to substract to the the polynomial.
 */
int fgp_msg_const_sub(fgp_msg * pol_out, fgp_msg * pol_in, bn_st * constant);

 /**
 * Multiplies a polyomial by the monome X:
 * \f$ \sum_{i=0}^{d} (c_i)·X^{i+1} \f$ if d is the polynomial degree and \f$c_i\f$
 * its coefficients (example with a polynomial of degree 0 in Y).
 * @param[out] pol			Polynomial to "increment"
 */
int fgp_msg_incr(fgp_msg * pol);

/**
 * Prints the polynomial to std output
 * @param[in] pol 		Pointer to the structure to print.
 */
int fgp_msg_print(fgp_msg * pol);

/**
 * Deallocates the coefficients of the polynomial.
 * @param[in] pol 		Pointer to the structure to clean.
 */
int fgp_msg_clean(fgp_msg * pol);

