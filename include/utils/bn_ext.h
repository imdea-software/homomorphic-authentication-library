#include <fmpz.h>
#include <fmpz_poly.h>
#include <fq.h>
#include <fq_poly.h>

#undef WORD
#undef CAT
#undef _CAT

#undef DIGIT
#define DIGIT 64

#define CAT(A, B)			_CAT(A, B)
#define _CAT(A, B)			A ## B

#include <relic/relic.h>

#if (FP_PRIME % 64) > 0
#define RELIC_DIGS	(FP_PRIME/64 + 1)
#else
#define RELIC_DIGS	(FP_PRIME/64)
#endif


#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>


#include "error_hdl.h"

/*
#undef WORD
#define WORD 64
*/

/*! \file bn_ext.h
    \brief An extension of the relic integer-handling functions
    
	These functions handle operations on integers or arrays of integers,
	as well as converstions and macros. The integers are all considered of size
	RELIC_DIGS in digits and reduced to the order of the elliptic curve groups.

    \ingroup utils
*/


// Defines a global mult_type constant which determines the kind of multiplication used:
// 		b -> basic
// 		k -> karatsuba
//		c -> comba

#ifndef MULT_TYPE
#define MULT_TYPE 'b'
#endif

#ifndef MOD_TYPE
#define MOD_TYPE 'b'
#endif

/**
 * Multiplies 2 integers.
 * @param[out] out 		Pointer to the result of the multiplication.
 * @param[in] in1 		Pointer to the first input.
 * @param[in] in2		Pointer to the second input.
 * @param[in] modulus 	Modulus to reduce to.
 */
int bn_mul_mod(bn_st * out, bn_st * in1, bn_st * in2, bn_st * modulus);

/**
 * Multiplies two arrays of size "size" together and reduces the result to modulus
 * @param[out] out 			Result of the multiplication.
 * @param[in] bn_array1		First input array.
 * @param[in] bn_array2		Second input array.
 * @param[in] size			Size of the arrays
 * @param[in] modulus 		Modulus to reduce to.
 */
int bn_mult_arrays_mod(bn_st * bn_out, bn_st * bn_array1, bn_st * bn_array2, int size, bn_st * modulus);

/**
 * Multiplies two arrays of size "size" together.
 * @param[out] out 			Result of the multiplication.
 * @param[in] bn_array1		First input array.
 * @param[in] bn_array2		Second input array.
 * @param[in] size			Size of the arrays
 */
int bn_mult_arrays(bn_st * bn_out, bn_st * bn_array1, bn_st * bn_array2, int size);

/**
 * Add two integers and reduce.
 * @param[out] out 		Pointer to the result of the addition.
 * @param[in] in1 		Pointer to the first input.
 * @param[in] in2		Pointer to the second input.
 * @param[in] modulus 	Modulus to reduce to.
 */
int bn_add_mod(bn_st * out, bn_st * in1, bn_st * in2, bn_st * modulus);

/**
 * Substract two integers and reduce.
 * @param[out] out 		Pointer to the result of the substraction.
 * @param[in] in1 		Pointer to the first input.
 * @param[in] in2		Pointer to the second input.
 * @param[in] modulus 	Modulus to reduce to.
 */
int bn_sub_mod(bn_st * out, bn_st * in1, bn_st * in2, bn_st * modulus);

/**
 * Squares an integer and reduce.
 * @param[out] out 		Pointer to the result of the square.
 * @param[in] in		Pointer to input to square.
 * @param[in] modulus 	Modulus to reduce to.
 */
int bn_sqr_mod(bn_st * out, bn_st * in, bn_st * modulus);

/**
 * Adds all the terms of an array to a single integer and reduces.
 * @param[out] out 			Pointer to the result of the addition.
 * @param[in] bn_array1		Input array.
 * @param[in] size			Size of the arrays
 * @param[in] modulus 		Modulus to reduce to.
 */
int bn_add_array_mod(bn_st * bn_out, bn_st * bn_array, int size, bn_st * modulus);

/**
 * Adds all the terms of an array to a single integer.
 * @param[out] out 			Pointer to the result of the addition.
 * @param[in] bn_array1		Input array.
 * @param[in] size			Size of the arrays.
 */
int bn_add_array(bn_st * bn_out, bn_st * bn_array, int size);

/**
 * Adds all the terms of an array to a single integer and reduces.
 * @param[out] inv_in		Pointer to the result of the inversion.
 * @param[in] in 			Pointer to the input.
 * @param[in] modulus		Order of the field in which to inverse.
 * @param[in] sizeof_mod	Size of the order in digits.
 */
int inverse(bn_st * inv_in, bn_st * in, bn_st * modulus, int sizeof_mod);

/**
 * Computes an affine function: y = a*x + b and reduce.
 * @param[out] y			Pointer to the result of the inversion.
 * @param[in] x 			Pointer to the input variable.
 * @param[in] a				Pointer to the first coefficient.
 * @param[in] b 			Pointer to the second coefficient.
 * @param[in] modulus 		Modulus to reduce to.
 */
int affine(bn_st * y, bn_st  * a, bn_st * x, bn_st * b, bn_st * mod);

/**
 * Allocates memory for an array of integers of size "size"
 * @param[in] array			Array to allocate.
 * @param[in] size 			Size of the array.
 */
int bn_new_array(bn_st * array, int size);

/**
 * Free memory for an array of integers of size "size"
 * The array is freed even of the size given is smaller than the actual
 * allocated memory.
 * @param[in] array			Array to free.
 * @param[in] size 			Size of the array.
 */
int bn_free_array(bn_st * array, int size);

/**
 * Sets an array of size "size" to zeros.
 * @param[in] array			Array to set to zero.
 * @param[in] size 			Size of the array.
 */
int bn_zero_array(bn_t * array, int size);

/**
 * Converts an mpz (gmp) integer to a bn (relic) integer 
 * @param[out] out			bn integer
 * @param[in] in 			mpz integer
 */
int mpz2bn(bn_t out, mpz_t in);

/**
 * Converts an fmpz (flint) integer to a bn (relic) integer 
 * @param[out] out			bn integer
 * @param[in] in 			mpz integer
 */
int fmpz2bn(bn_t out, fmpz_t in);

/**
 * Converts a bn (relic) integer to an mpz (gmp integer) 
 * @param[out] out			mpz integer
 * @param[in] in 			bn integer
 */
int bn2mpz(mpz_t out, bn_t in);

/**
 * Converts a bn (relic) integer to an fmpz (flint integer)
 * @param[out] out			mpz integer
 * @param[in] in 			bn integer
 */
int bn2fmpz(fmpz_t out, bn_t in);


/**
 * Normalize the add function from the relic elliptic curve for G1.
 * @param[out] constants 	Result of the addition and normalization.
 * @param[in] in1 	 		First input.
 * @param[in] in2			Second input.
 */
int g1_add_norm(g1_t out, g1_t in1, g1_t in2);

/**
 * Normalize the add function from the relic elliptic curve for G2.
 * @param[out] constants 	Result of the addition and normalization.
 * @param[in] in1 	 		First input.
 * @param[in] in2			Second input.
 */
int g2_add_norm(g2_t out, g2_t in1, g2_t in2);

/**
 * Converts an unsigned integer to an array of 4 unsigned bytes.
 * @param[out] out 			Pointer to the output array.
 * @param[in] in 	 		Unsigned integer
 */
int int2uint8_t(uint8_t * out, int in);

/**
 * Converts a string of 4 unsigned bytes to an integer.
 * @param[out] out 			Output integer.
 * @param[in] in 	 		Pointer to the byte array to convert.
 */
int uint8_t2int(int * out, uint8_t * in);

/**
 * Converts an unsigned integer to a bn integer.
 * @param[out] out 			Output bn integer.
 * @param[in] in 	 		Unsigned integer
 */
int int2bn(bn_t out, int in);

/**
 * Converts a signed integer to a bn integer.
 * @param[out] out 			Output bn integer.
 * @param[in] in 	 		Signed integer
 * @param[in] size 	 		The size of the integer in base 10.
 */
int sint2bn(bn_t out, int in, int size);

/**
 * Converts a bn integer to an int.
 * @param[out] out 			Output bn integer.
 * @param[in] in 	 		Signed integer
 * @param[in] size 	 		The size of the integer in base 10.
 */
int bn2int(int * out, bn_t in);

/**
 * Calls the g1_get_ord() RELIC function to get the order of the elliptic curve groups and return it as an fmpz int.
 * @param[out] mod 			The order of the elliptic curve groups.
 */
int mod_set_fmpz(fmpz_t mod);

/**
 * Converts a bn integer to an fq integer, given a certain finite field context.
 * @param[out] out 			Output fq integer.
 * @param[in] in 	 		Input bn integer.
 * @param[in] ctx 	 		Finite field context.
 */
int bn2fq(fq_t out, bn_t in, fq_ctx_t ctx);

/**
 * Converts an fq integer to a bn integer given the context of the fq finite field.
 * @param[out] out 			Output bn integer.
 * @param[in] in 	 		Input fq integer.
 * @param[in] ctx 	 		Finite field context.
 */
int fq2bn(bn_t out, fq_t in, fq_ctx_t ctx);



