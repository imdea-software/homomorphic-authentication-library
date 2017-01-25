#include "utils.h"


/*! \file prf.h
    \brief File describing the pseudo random function and sub-functions it uses
    
	The prf is created through an hmac using different hashes depending on the macro HASH_LEN
	and the result is expanded to size EXP_LEN and then reduced to the appropriate output size.

    \ingroup utils
*/


/*! \def HASH_LEN
    \brief Gives the lentgh of the hash used for the prf.
   
	These are the different possible hashes and their corresponding length;
	\f[
	 \begin{array}{c|rl}
 	 20 & sha & one  \\
 	 28 & sha & 224  \\
 	 32 & sha & 256  \\
 	 48 & sha & 384  \\
 	 64 & sha & 512 
 	 \end{array}
	\f]
*/
#ifndef HASH_LEN
#define HASH_LEN 32
#endif


/*! \def ENC_TYPE
    \brief Gives the type of encryption used for the prf.
   
	Has to be set to one for one !
	(1 is for hmac and 2 for aes) 
*/
#ifndef ENC_TYPE
#define ENC_TYPE 1
#endif



/**
 * Computes an hmac of the input byte vector with the byte vector key in this fashion:
 * First, the key is expanded (addition of 0x00) or reduceded (hashed) to the hash block size
 * stored in the BLOCK_SIZE variable. \n
 * Then, two versions of the key are used: 
 * \f$ k_i = K \oplus (0x36) \f$ and \f$ k_o = K \oplus (0x5c) \f$ \n
 * Then the hmac outputs: 
 * \f$
 *  (k_o || h(k_i || m)) 
 * \f$
 * where m is the message, || is concatenation and h is hash function used.
 * @param[out] out 			Output byte vector.
 * @param[in] out_size 	 	Size of the output.
 * @param[in] input 		Input byte vector.
 * @param[in] in_size 	 	Size of the input.
 * @param[in] key 	 		The key as a byte vector.
 * @param[in] key_size 		The size of the byte vector key.
 */
int hmac(uint8_t * out, int out_size, uint8_t * input, int in_size, uint8_t * key, int key_size);

/**
 * Unused function designed to create a pseudo-random function through aes.
 * @param[out] out 			Output byte vector.
 * @param[in] out_size 	 	Size of the output.
 * @param[in] input 		Input byte vector.
 * @param[in] in_size 	 	Size of the input.
 * @param[in] key 	 		The key as a byte vector.
 * @param[in] key_size 		The size of the byte vector key.
 */
int aes(uint8_t * out, int out_size, uint8_t * input, int in_size, uint8_t * key, int key_size);

/**
 * A macro for a pseudo-random fucntion. As of now, this is just a call to hmac.
 * @param[out] out 			Output byte vector.
 * @param[in] out_size 	 	Size of the output.
 * @param[in] input 		Input byte vector.
 * @param[in] in_size 	 	Size of the input.
 * @param[in] key 	 		The key as a byte vector.
 * @param[in] key_size 		The size of the byte vector key.
 */
int prf(uint8_t * out, int out_size, uint8_t * input, int in_size, uint8_t * key, int key_size);

/**
 * A macro for the hash function to use. This will depend on the global variable
 * HASH_LEN which is fixed as of now but could be set at configuration of the library
 * in an other version.
 * @param[out] hash 		Output hash as a byte vector.
 * @param[in] msg 	 		Input hash as a byte vector.
 * @param[in] length 	 	The length of the input byte vector.
 */
int hash_func(uint8_t * hash, uint8_t * msg, int length);

/**
 * Our goal in the decouple function is to create two numbers in \f$ [1, q] \f$. The result from the 
 * hmac based on sha256 (function hmac(...)) therefore has to be reduced in some way.
 * For this, the result of the hmac is expanded to a certain number of bits
 * in order for the subsequent reduction modulo the order q to have as little redundancy as possible
 * and for the result to keep its pseudo random property. \n
 * In effect, with q the order of the bilinear groups, we are looking to make the statistical approximation:
 * \f[
 *  [1,q] \sim [1,2^{\lambda'}]
 * \f]
 * Where any element of \f$ [1,2^{\lambda'}] \f$ would be reduced modulo q. To this effect,
 * we are looking to minimize the statistical distance between those two sets. This translates to the formula:
 * \f[
 * \sum_{i=1}^{q} |P(X_2 = i) - P(X_1 [q] = i))| \leq 2^{-\lambda}
 * \f]
 * This function expands the inout to the desired number of bits, calculated by the above formula
 * (this number is stored in the global variable EXP_LEN).
 * @param[out] out 			Output byte vector.
 * @param[in] in 	 		Input byte vector to expand.
 * @param[in] in_size 	 	The size of the input.
 * @param[in] bit_number 	The number of bits to expand to (EXP_LEN in the use for the decoupling function)
 */
int expand(uint8_t * out, uint8_t * in, int in_size, int bit_number);