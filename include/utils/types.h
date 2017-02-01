#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <fmpz.h>

#include "error_hdl.h"

/*! \file types.h
    \brief Handling of uint8_t, char and string types
    
	The file contains conversions and string operations relative
	to the "string" "uint8_t" and "char" types

    \ingroup utils
*/

// Max strinf size of inputs such as labels and delta
#ifndef MAX_STR_SIZE
#define MAX_STR_SIZE 1000
#endif


/**
 * Separate the input array into two byte arrays.
 * @param[out] out1 			First half of the input array.
 * @param[out] out2 			Second half of the input array.
 * @param[in] input 			Input byte array
 * @param[in] in_size 			Size of the input array
 */
int separate(uint8_t * out1, uint8_t * out2, uint8_t * input, int in_size);

/**
 * Xors 2 hex strings of the same size into another one 
 * @param[out] out 				Output array.
 * @param[in] in1 				First array.
 * @param[in] in2 				Second array.
 */
int xor_hex (unsigned char * out, unsigned char * in1, unsigned char * in2);

/**
 * Xors 2 byte strings of the same size into another one 
 * @param[out] out 				Output array.
 * @param[in] in1 				First array.
 * @param[in] in2 				Second array.
 */
int xor_byte (uint8_t * out, uint8_t * in1, uint8_t * in2, int size);

/**
 * Gives the hex value of a character.
 * @param[in] c 				Input character. Has to be either 1..9, a..f
 * @return 						The hex value as an unsigned int.
 */
unsigned int value(char c);

/**
 * Get a character with the hax value of the input int.
 * @param[in] nb 				Input int. Has t be \f$ nb < 16 \f$
 * @return 						The character as an unsigned char
 */
unsigned char hex_value(int nb);

/**
 * Concatenates two byte arrays together in that order.
 * @param[out] out 				Output array.
 * @param[in] size_out 			Size of the output
 * @param[in] in1 				First input array.
 * @param[in] size1		 		Size of the first input.
 * @param[out] in2 				Second input array.
 * @param[in] size2		 		Size of the second input.
 */
int concatenate (uint8_t * out, int size_out, uint8_t * in1, int size1, uint8_t * in2, int size2);

/**
 * Tranforms a string of characters into a byte array. 
 * @param[out] size 			Pointer to the size of the returned array.
 * @param[in] char_array 		Pointer to the array of characters to convert.
 * @return 						Pointer to the array of bytes.
 */
uint8_t * char_to_uint8_t (int * size, char * char_array);

/**
 * Tranforms a byte array into a string.
 * @param[out] char_array 		Array of characters.
 * @param[in] byte_array 		Array of bytes.
 * @param[in] size 				Size of the input byte array.
 */
int uint8_t_to_char (char * char_array, uint8_t * byte_array, int size);

/**
 * Tranforms a byte array into a stringof hex.
 * @param[out] byte_array 		Array of bytes.
 * @param[in] hex_array 		Array of hex characters.
 * @param[in] byte_size 		Size IN BYTES of the input hex string (size of the string / 2)
 */
int hex_to_uint8_t (uint8_t * byte_array , char * hex_array, int byte_size);

/**
 * Tranforms a byte array into a stringof hex.
 * @param[out] hex_array 		Array of hex characters.
 * @param[in] byte_array 		Pointer to the size of the returned array.
 * @param[in] byte_size 		Size of the input byte array.
 */
int uint8_t_to_hex (char * hex_array, uint8_t * byte_array , int byte_size);

/**
 * Tranforms a byte array into an fmpz_t integer.
 * @param[out] out_v			fmpz_t integer.
 * @param[in] byte_array 		Pointer to the byte array.
 * @param[in] byte_size 		Size of the input byte array.
 */
int uint8_t_to_fmpz_t (fmpz_t out_v, uint8_t * byte_array , int byte_size);

/**
 * Copy the nb_bits first BITS of in into out.
 * @param[out] out 		 		Output byte array.
 * @param[in] in 		 		Input byte array.
 * @param[in] nb_bits 			Number of BITS copied.
 */
int uint8_t_copy(uint8_t * out, uint8_t in, int nb_bits);

/**
 * Returns the max of two ints.
 * @param[in] in1 		 		First int.
 * @param[in] in2 		 		Second int.
 * @return 			 			Max of the two integers.
 */
int max(int in1, int in2);

/**
 * Creates a string containing the integer j. Must have been allocated before.
 * @param[out] out  		 	The string.
 * @param[in] j 		 		The int to fill the string with.
 */
int label(char * out, int j);
