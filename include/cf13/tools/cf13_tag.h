#include "utils.h"
#ifndef _cf13_keys_H_
#define _cf13_keys_H_
#include "cf13_keys.h"
#endif
#ifndef _cf13_msg_H_
#define _cf13_msg_H_
#include "cf13_msg.h"
#endif

/*! \file cf13_tag.h
    \brief File describing the Tag structure.
 
    \ingroup tools
*/


/*! \struct cf13_tag
 * A tag is a structure composed of a \f$Z_q[X]\f$ polynomial y.
 */
typedef struct
{
	fq_ctx_t ctx;
	
	fq_poly_t y;
}cf13_tag;


/**
 * Allocates memory for a tag, setting the degree to 1.
 * @param[in] s 	Pointer to the structure to allocate.
 */
int cf13_tag_new(cf13_tag * s, cf13_private_key *sk);

/**
 * Free the structure.
 * @param[in] s 	Pointer to the structure to free.
 */
int cf13_tag_free(cf13_tag *s);

/**
 * Copy one tag into another.
 * @param[out] s_out 	Pointer to the structure to copy into.
 * @param[in] s_in 		Pointer to the structure to copy from.
 */
int cf13_tag_copy(cf13_tag * s_out, cf13_tag * s_in);

/**
 * Multiply a constant to a tag.
 *
 * @param[out] s_out 	Resulting tag.
 * @param[in] s_in 		cf13_tag to multiply.
 * @param[in] c 		cf13_msg to multiply.
 */
int cf13_tag_const_mult(cf13_tag *s_out, cf13_tag *s_in, cf13_msg * c);

/**
 * Add a constant to a tag. 
 *
 * @param[out] s_out 	Resulting tag.
 * @param[in] s_in 		cf13_tag to add.
 * @param[in] c 		cf13_msg constant to add.
 */
int cf13_tag_const_add(cf13_tag *s_out, cf13_tag *s_in, cf13_msg * c);


/**
 * Multiply two tags together.
 *
 * @param[out] s 		Resulting tag.
 * @param[in] s1 		First inpput.
 * @param[in] s2 		Second input.
 */
int cf13_tag_mul(cf13_tag * s, cf13_tag * s1, cf13_tag * s2);

/**
 * Add two tags together.
 * @param[out] s 		Resulting tag.
 * @param[in] s1 		First inpput.
 * @param[in] s2 		Second input.
 */
int cf13_tag_add(cf13_tag * s, cf13_tag * s1, cf13_tag * s2);

/**
 * Print the tag specifications (degree) and variables to
 * standard output.
 * @param[in] s 	Pointer to the tag to print.
 */
int cf13_tag_print(cf13_tag *s);

/**
 * Write a tag to file
 * @param[out] file 	File to write to.
 * @param[in] s 		Pointer to the tag to write.
 */
int cf13_tag_write(unsigned char * file, cf13_tag * s);

/**
 * Read any tag from file
 * @param[out] s 		Pointer to the tag to read into.
 * @param[int] file 	File to read from.
 */
int cf13_tag_read(cf13_tag * s, unsigned char * file);

/**
 * Set a tag to zero (degree 1)
 * @param[out] s 		Pointer to the tag to set.
 */
int cf13_tag_zero(cf13_tag * s);
