#ifndef _FGP_PRF_H_
#define _FGP_PRF_H_
#include "fgp_prf.h"
#endif

/*! \file fgp_tag.h
    \brief File describing the Tag structure.
    
	A tag is an object authenticating a message (polynomial) and a private key.
	A tag can be of degree 1 or 2, where the degree corresponds to the degree of
	variable Y in the authenticated polynomial.

    \ingroup tools
*/


/*! \struct fgp_tag
 * A tag is a structure composed of \f$G_1, G_2, G_t\f$ elements where those are 
 * groups of the same prime order \f$ q \f$. \f$ g \in G_1 \f$ and \f$h \in G_2\f$ are two generators,
 * and \f$ e: G_1 \times G_2 \f$ is a non-degenerate bilinear map. \n
 * A tag can be either of degree 1 (created from authentication or addition of degree 1 tags) or degree 2
 * (multiplication of 2 degree 1 tags for instance).
 * The group 1 and 2 elements and the lambda are the ones that define a degree 1 tag while the three pairing group
 * elements define the degree 2 tag. 
 * In the following comments, a tag will be written \f$ \sigma = (T, U, X, Y, \lambda)\f$ where U and Y don't matter
 * if the tag is of degree two and then X is s->X2 and T is s->T2.
 */
typedef struct
{
	// Degree can be either 1 or 2
	int degree;

	g1_st T;
	g2_st U;
	g1_st X;
	g2_st Y;

	gt_t T2;	// T for degree 2
	gt_t X2;	// X for degree 2
	gt_t lambda;
}fgp_tag;


/**
 * Allocates memory for a tag, setting the degree to 1.
 * @param[in] s 	Pointer to the structure to allocate.
 */
int fgp_tag_new(fgp_tag * s);

/**
 * Free the structure.
 * @param[in] s 	Pointer to the structure to free.
 */
int fgp_tag_free(fgp_tag *s);

/**
 * Clean the memory of the strucure variables. Keep the pointer to the structure.
 * @param[in] s 	Pointer to the structure to clean.
 */
int fgp_tag_clean(fgp_tag *s);

/**
 * Copy one tag into another.
 * @param[out] s_out 	Pointer to the structure to copy into.
 * @param[in] s_in 		Pointer to the structure to copy from.
 */
int fgp_tag_copy(fgp_tag * s_out, fgp_tag * s_in);

/**
 * Multiply a constant to a tag.
 * Let \f$ \sigma_o = (T_o, U_o, X_o, Y_o, \lambda_o)\f$ be the output sigma
 * and \f$ \sigma_i = (T_i, U_i, X_i, Y_i, \lambda_i)\f$ the input sigma.
 * Here are the operations:
 *  \f[
 * 	\begin{array}{ccc}
 *	T_o & \leftarrow &  T_i^c  \\
 *	U_o & \leftarrow &  U_i^c  \\
 *	X_o & \leftarrow &  X_i^c  \\
 *	Y_o & \leftarrow &  Y_i^c  \\
 *	\lambda_o & \leftarrow &  \lambda_i^c 
 *	\end{array}
 *	\f]
 * @param[out] s_out 	Resulting tag.
 * @param[in] s_in 		fgp_tag to multiply.
 * @param[in] c 		Constant to multiply.
 */
int fgp_tag_const_mult(fgp_tag *s_out, fgp_tag *s_in, bn_st * c);

/**
 * Add a constant to a tag. The method depends on the degree of the tag.
 * Let \f$ \sigma_o = (T_o, U_o, X_o, Y_o, \lambda_o)\f$ be the output sigma
 * and \f$ \sigma_i = (T_i, U_i, X_i, Y_i, \lambda_i)\f$ the input sigma.
 * Here are the operations:
 *  \f[
 * 	\begin{array}{ccc|ccc}
 * 	deg(\sigma_i) & = & 1 & deg(\sigma_i) & = &  2 \\
 *	&&&&& \\
 *	T_o & \leftarrow &  T_i \times g^c & T & \leftarrow & T_i \times e(g,h)^c \\
 *	U_o & \leftarrow &  U_i \times h^c &&&\\
 *	X_o & \leftarrow &  X_i & X_o & \leftarrow & X_i \\
 *	Y_o & \leftarrow &  Y_i &&&\\
 *	\lambda_o & \leftarrow &  \lambda_i & \lambda_o & \leftarrow &  \lambda_i 
 *	\end{array}
 *	\f]
 * @param[out] s_out 	Resulting tag.
 * @param[in] s_in 		fgp_tag to add.
 * @param[in] c 		Constant to add.
 */
int fgp_tag_const_add(fgp_tag *s_out, fgp_tag *s_in, bn_st * c);

/**
 * Substract a constant to the tag (take the modular opposite of the constant before
 * applying the constant addition function).
 * @param[out] s_out 	Resulting tag.
 * @param[in] s_in 		tag to substract from.
 * @param[in] c 		Constant to substract.
 */
int fgp_tag_const_sub(fgp_tag *s_out, fgp_tag *s_in, bn_st * c);

/**
 * Multiply two tags together.
 * If any of the input tags is of degree 2, an error is thrown: the structure
 * only allows for degree 1 tag multiplication.
 * In effect, the computation is as follows with \f$ \sigma = (T, U, X, Y, \lambda)\f$ as the output sigma:
 * We are multiplying:
 * 	\f[
 *	\begin{array}{ccc}
 *	\sigma_1 = (T_1, U_1, X_1, Y_1, \lambda_1) & * & \sigma_2 = (T_2, U_2, X_2, Y_2, \lambda_2)
 *	\end{array}
 *	\f]
 *	\f[
 * 	\begin{array}{ccc}
 * 	T &  \leftarrow & e(T_1, U_2) \\
 * 	\Lambda & \leftarrow & e(X_1, Y_2) \\ 
 * 	X & \leftarrow & e(X_1, U_2) e(X_2, U_1) 
 *	\end{array}
 *	\f]
 * @param[out] s 		Resulting tag.
 * @param[in] s1 		First inpput.
 * @param[in] s2 		Second input.
 */
int fgp_tag_mul(fgp_tag * s, fgp_tag * s1, fgp_tag * s2);

/**
 * Add two tags together.
 * The addition takes different forms depending on the degrees of the input tags.
 * Let \f$ \sigma = (T, U, X, Y, \lambda)\f$ be the output sigma (if it is of degree 2
 * U and Y are irrelevant and X and Y are represented in the structure by X2 and T2).
 * We are adding:
 * 	\f[
 *	\begin{array}{ccc}
 *	\sigma_1 = (T_1, U_1, X_1, Y_1, \lambda_1) & + & \sigma_2 = (T_2, U_2, X_2, Y_2, \lambda_2)
 *	\end{array}
 *	\f]
 * Here are the operations for the different cases. Note that the case where both inputs are of degree 2
 * is the same as degree 1 in that U and Y just don't matter. Here, \f$h\f$ is the generator of the elliptic
 * group 2.
 *  \f[
 * 	\begin{array}{ccc|ccc}
 * 	deg(\sigma_1) & = & deg(\sigma_2) & deg(\sigma_1) = 1 & \& & deg(\sigma_2) = 2 \\
 *	&&&&& \\
 *	T & \leftarrow & T_1 * T_2 & T & \leftarrow & e(T_1,h) * T_2 \\
 *	U & \leftarrow &  U_1 * U_2 &&&\\
 *	X & \leftarrow &  X_1 * X_2 & X & \leftarrow & e(X_1,h) * X_2 \\
 *	Y & \leftarrow &  Y_1 * Y_2 &&&\\
 *	\lambda & \leftarrow &  \lambda_1 * \lambda_2 & \lambda & \leftarrow &  \lambda_1 * \lambda_2 
 *	\end{array}
 *	\f]
 * @param[out] s 		Resulting tag.
 * @param[in] s1 		First inpput.
 * @param[in] s2 		Second input.
 */
int fgp_tag_add(fgp_tag * s, fgp_tag * s1, fgp_tag * s2);

/**
 * Print the tag specifications (degree) and variables to
 * standard output.
 * @param[in] s 	Pointer to the tag to print.
 */
int fgp_tag_print(fgp_tag *s);

/**
 * Write a degree 1 tag to file
 * @param[out] file 	File to write to.
 * @param[in] s 		Pointer to the tag to write.
 */
int fgp_tag_write1(unsigned char * file, fgp_tag * s);

/**
 * Write a degree 2 tag to file
 * @param[out] file 	File to write to.
 * @param[in] s 		Pointer to the tag to write.
 */
int fgp_tag_write2(unsigned char * file, fgp_tag * s);

/**
 * Write any tag to file
 * @param[out] file 	File to write to.
 * @param[in] s 		Pointer to the tag to write.
 */
int fgp_tag_write(unsigned char * file, fgp_tag * s);

/**
 * Read any tag from file
 * @param[out] s 		Pointer to the tag to read into.
 * @param[int] file 	File to read from.
 */
int fgp_tag_read(fgp_tag * s, unsigned char * file);

/**
 * Read a degree 1 tag from file
 * @param[out] s 		Pointer to the tag to read into.
 * @param[int] file 	File to read from.
 */
int fgp_tag_read1(fgp_tag * s, unsigned char * file);

/**
 * Read a degree 2 tag from file
 * @param[out] s 		Pointer to the tag to read into.
 * @param[int] file 	File to read from.
 */
int fgp_tag_read2(fgp_tag * s, unsigned char * file);

/**
 * Set a tag to zero (degree 1)
 * @param[out] s 		Pointer to the tag to set.
 */
int fgp_tag_zero(fgp_tag * s);
