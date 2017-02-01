#include "utils.h"


/*! \file rng.h
    \brief File describing basic random number generation functions
 
    \ingroup utils
*/

/**
 * Returns random bytes
 * @param[out] v 			Output byte vector.
 * @param[in] size			The size of the byte vector v.
 */
int hal_get_rand_bytes(uint8_t * v, int size);

/**
 * Returns a random value in the field F_q (assuming q to be prime).
 *
 * The function uses hal_get_rand_bytes to get a random string of
 * |q| bits and tries until it gets an integer in [0,q) (rejection sampling).
 *
 * @param[out] r 			Output fq_t value.
 * @param[in] ctx			flint fq context.
 */
int hal_get_rand_fq(fq_t r, fq_ctx_t ctx);

/**
 * Returns a random integer in the range [0, 2^bits -1].
 *
 * @param[out] r 			Output fmpz_t value.
 * @param[in] bits			Logarithmic bound so that r<2^bits.
 */
int hal_get_randbits_fmpz(fmpz_t r, int bits);

/**
 * Returns a random integer of bits length, i.e., 
 * in the range [2^{bits-1}, 2^bits -1].
 *
 * @param[out] r 			Output fq_t value.
 * @param[in] bits			length in bits of the integer to generate.
 */
int hal_get_randlen_fmpz(fmpz_t r, int bits);
