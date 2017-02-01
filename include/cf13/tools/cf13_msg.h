#include "utils.h"
#ifndef _cf13_keys_H_
#define _cf13_keys_H_
#include "cf13_keys.h"
#endif

/*! \file cf13_msg.h
    \brief File describing the CF13 messages.
 
    \ingroup tools
*/

/*! \struct cf13_msg
 */
typedef struct
{
	fq_ctx_t ctx;

	fq_t msg;

} cf13_msg;


/**
 * Message allocation. Allocates an empty message. The message created is set to 0.
 * @param[in] m 		Pointer to a message to allocate.
 */
int cf13_msg_new(cf13_msg * m, cf13_public_key *pk);

/**
 * Frees the structure entirely.
 * @param[in] m 		Pointer to the structure to free.
 */
int cf13_msg_free(cf13_msg * m);

/**
 * Write a message to file
 * @param[out] file_name 	File to write to.
 * @param[in] m 			Pointer to the message to write.
 */
int cf13_msg_write(unsigned char * file_name, cf13_msg * m);

/**
 * Read a message from file
 * @param[out] m			Pointer to the message to read into.
 * @param[int] file_name 	File to read from.
 */
int cf13_msg_read(cf13_msg * m, unsigned char * file_name);

/**
 * Create a random message.
 * @param[out] m 			Pointer to the message to set.
 */
int cf13_msg_rand(cf13_msg * m);

/**
 * Add two messages together
 * @param[out] m_out 		Pointer to the output message.
 * @param[in] m1			First input.
 * @param[in] m2			Second input.
 */
int cf13_msg_add(cf13_msg * m_out, cf13_msg * m1, cf13_msg * m2);

/**
 * Copy a message into another.
 * @param[out] m_out 		Pointer to the output message.
 * @param[in] m_in		Message to copy from.
 */
int cf13_msg_copy(cf13_msg * m_out, cf13_msg * m_in);

/**
 * Multiply two messages together
 * @param[out] m_out 		Pointer to the output message.
 * @param[in] m1			First input.
 * @param[in] m2			Second input.
 */
 int cf13_msg_mul(cf13_msg * m_out, cf13_msg * m1, cf13_msg * m2);


/**
 * Prints the message to std output
 * @param[in] m 		Pointer to the structure to print.
 */
int cf13_msg_print(cf13_msg * m);

