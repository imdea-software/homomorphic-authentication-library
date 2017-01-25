#ifndef _FGP_PRF_H_
#define _FGP_PRF_H_
#include "fgp_prf.h"
#endif

#ifndef _fgp_keys_H_
#define _fgp_keys_H_
#include "fgp_keys.h"
#endif

#ifndef _PRF_H_
#define _PRF_H_
#include "prf.h"
#endif

/*! \file fgp_verification_keys.h
    \brief File describing the verification fgp_keys.
    
	Verification fgp_tags are bivariate polynomials of degree 2 or less.  

    \ingroup tools
*/

/*! \struct fgp_vkf
 *  Bivariate polynomials of degree 2 or less.
 *	The polynomial takes the from: \f$ r(A,B) = c + c_{a}·A + c_{b}·B + c_{ab}·A·B + c_{a^2}·A^2 + c_{b^2}·B^2\f$.\n
 * 	A basic verification key is created with degree with two coefficients \f$ c_a,c_b \f$ outputs of the prf.
 *	The degree two is obtained through multplication by two degree one fgp_keys, and the constant term corresponds to an addtion by constant.
 */
typedef struct
{
	int degree;
	bn_st mod;			// The order of the elliptic fields
				
	bn_st C;			// Constant		
	bn_st Acoef;
	bn_st Bcoef;
	bn_st ABcoef;
	bn_st A2coef;
	bn_st B2coef;
	
} fgp_vkf;

/**
 * Allocate memory for all the coefficients of the key and set the modulus variable.
 * @param[in] r 		Pointer to the fgp_vkf to allocate.
 */
int fgp_vkf_new(fgp_vkf * r);

/**
 * Deallocate memory for all the coefficients and free the strucuture pointer.
 * @param[in] r 		Pointer to the fgp_vkf to deallocate.
 */
int fgp_vkf_free(fgp_vkf *r);

/**
 * Deallocate memory for all the coefficients but don't free the strucuture pointer.
 * @param[in] r 		Pointer to the fgp_vkf to deallocate.
 */
int fgp_vkf_clean(fgp_vkf *r);

/**
 * Copy one fgp_vkf into antoher.
 * @param[out] r_out 		Key to copy into.
 * @param[in] r_in 			Key to copy from.
 */
int fgp_vkf_copy(fgp_vkf *r_out, fgp_vkf *r_in);

/**
 * Add two fgp_keys together (polynomial addition).
 * @param[out] r_out 		Output key.
 * @param[in] r1 			First input.
 * @param[in] r2 			Second input.
 */
int fgp_vkf_add(fgp_vkf * r_out, fgp_vkf * r1, fgp_vkf * r2);

/**
 * Multiply a two fgp_keys together (polynomial multiplication). The two fgp_keys
 * have to be of degree 1 for the result to be correct. Otherwise, the result
 * is the degree 1 truncated resut of the multiplication.
 * @param[out] r_out 		Output key.
 * @param[in] r1 			First input.
 * @param[in] r2 			Second input.
 */
int fgp_vkf_mul(fgp_vkf *r_out, fgp_vkf *r1, fgp_vkf *r2);

/**
 * Multiply a key by a constant (normal modular multiplication).
 * @param[out] r_out 		Output key.
 * @param[in] r_in 			Input key.
 * @param[in] constant 		Constant to multiply by.
 */
int fgp_vkf_const_mult(fgp_vkf * r_out, fgp_vkf * r_in, bn_st * constant);

/**
 * Add a constant to a key. Simple modular addition.
 * @param[out] r_out 		Output key.
 * @param[in] r_in 			Input key.
 * @param[in] constant 		Constant to add.
 * @param[in] key 			Secret key.
 */
int fgp_vkf_const_add(fgp_vkf * r_out, fgp_vkf * r_in, bn_t constant);

/**
 * Substract a constant to a key. Simple modular substract.
 * @param[out] r_out 		Output key.
 * @param[in] r_in 			Input key.
 * @param[in] constant 		Constant to substract.
 * @param[in] key 			Secret key.
 */
int fgp_vkf_const_sub(fgp_vkf * r_out, fgp_vkf * r_in, bn_t constant);

/**
 * Evaluate a key in two values (polynomial evaluation).
 * @param[out] output 		Output integer.
 * @param[in] r 			Key to evaluate.
 * @param[in] in1 			First input: the "A" coefficient.
 * @param[in] in2 			Second input: the "B" coefficient.
 */
int fgp_vkf_eval(bn_st * output, fgp_vkf * r, bn_st * in1, bn_st * in2);

/**
 * Set a verification key to a degree 1 key with coefficients from the evaluation of
 * the string L by the pseudo random function initialized with the key key.
 * @param[out] r 			The initialized verification key.
 * @param[in] key 			The key.
 * @param[in] L 			The label that goes through the prf.
 */
int fgp_vkf_set(fgp_vkf * r, fgp_private_key * key, char *  L);

/**
 * Sets a verification to zero and degree to 1.
 * @param[in] r 		Pointer to the vkf to set.
 */
int fgp_vkf_zero(fgp_vkf *r);

/**
 * Print the fgp_vkf specifications (degree) and variables to
 * standard output.
 * @param[in] r 	Pointer to the fgp_vkf to print.
 */
int fgp_vkf_print(fgp_vkf *r);

/**
 * Write any fgp_vkf to file
 * @param[out] file 	File to write to.
 * @param[in] r 		Pointer to the fgp_vkf to write.
 */
int fgp_vkf_write(unsigned char * file, fgp_vkf * r);

/**
 * Write a degree 2 fgp_vkf to file
 * @param[out] file 	File to write to.
 * @param[in] r 		Pointer to the fgp_vkf to write.
 */
int fgp_vkf_write2(unsigned char * file, fgp_vkf * r);

/**
 * Write a degree 1 fgp_vkf to file
 * @param[out] file 	File to write to.
 * @param[in] r 		Pointer to the fgp_vkf to write.
 */
int fgp_vkf_write1(unsigned char * file, fgp_vkf * r);

/**
 * Read a degree 1 fgp_vkf from file
 * @param[out] s 		Pointer to the fgp_vkf to read into.
 * @param[int] file 	File to read from.
 */
int fgp_vkf_read1(fgp_vkf * r, unsigned char * file);

/**
 * Read a degree 2 fgp_vkf from file
 * @param[out] s 		Pointer to the fgp_vkf to read into.
 * @param[int] file 	File to read from.
 */
int fgp_vkf_read2(fgp_vkf * r, unsigned char * file);

/**
 * Read any fgp_vkf from file
 * @param[out] s 		Pointer to the fgp_vkf to read into.
 * @param[int] file 	File to read from.
 */
int fgp_vkf_read(fgp_vkf * r, unsigned char * file);

