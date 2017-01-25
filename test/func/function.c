/*****Automatically generated file *****/

#include "function.h"
/* 

Write a function in C that takes as inputs: 
	--> fgp_tag ** inputs 
	--> int size
And as output:
	-->fgp_tag * output

You have a certain number of generic functions at your disposal:
	- int fgp_tag_add(fgp_tag * out, fgp_tag * in1, fgp_tag * in2)
	- int fgp_tag_mul(fgp_tag * out, fgp_tag * in1, fgp_tag * in2)
	- int fgp_tag_copy(fgp_tag * out, fgp_tag * in)
	- int fgp_tag_new(fgp_tag * element)
	- int fgp_tag_free(fgp_tag * element)

To create a new fgp_tag element,
	fgp_tag * element = malloc(sizeof(*element));
	fgp_tag_new(element);
To free a fgp_tag element,
	fgp_tag_free(element);

*/


int func_fgp_tag (fgp_tag * output, fgp_tag ** inputs, bn_t * coefficients, int size)
{

	/***** WRITE YOUR CODE BELOW *****/

	// Temporary variable
	fgp_tag * temp = malloc(sizeof(*temp));
	fgp_tag_new(temp);

	fgp_tag_copy(output, inputs[0]);
	for (int i = 1; i <  size ; ++i)
	{
		fgp_tag_add(output, output, inputs[i]);
	}
	for (int i = 1; i <  size; ++i)
	{
		fgp_tag_mul(temp, inputs[i-1], inputs[i]);
		fgp_tag_const_mult(temp, temp, coefficients[i]);
		fgp_tag_add(output, output, temp);
	}

	// Clean up

	fgp_tag_free(temp);

	/**** END OF WRITING, DON'T EVEN DARE ******/


	return 0;
}
/* 

Write a function in C that takes as inputs: 
	--> fgp_vkf ** inputs 
	--> int size
And as output:
	-->fgp_vkf * output

You have a certain number of generic functions at your disposal:
	- int fgp_vkf_add(fgp_vkf * out, fgp_vkf * in1, fgp_vkf * in2)
	- int fgp_vkf_mul(fgp_vkf * out, fgp_vkf * in1, fgp_vkf * in2)
	- int fgp_vkf_copy(fgp_vkf * out, fgp_vkf * in)
	- int fgp_vkf_new(fgp_vkf * element)
	- int fgp_vkf_free(fgp_vkf * element)

To create a new fgp_vkf element,
	fgp_vkf * element = malloc(sizeof(*element));
	fgp_vkf_new(element);
To free a fgp_vkf element,
	fgp_vkf_free(element);

*/


int func_fgp_vkf (fgp_vkf * output, fgp_vkf ** inputs, bn_t * coefficients, int size)
{

	/***** WRITE YOUR CODE BELOW *****/

	// Temporary variable
	fgp_vkf * temp = malloc(sizeof(*temp));
	fgp_vkf_new(temp);

	fgp_vkf_copy(output, inputs[0]);
	for (int i = 1; i <  size ; ++i)
	{
		fgp_vkf_add(output, output, inputs[i]);
	}
	for (int i = 1; i <  size; ++i)
	{
		fgp_vkf_mul(temp, inputs[i-1], inputs[i]);
		fgp_vkf_const_mult(temp, temp, coefficients[i]);
		fgp_vkf_add(output, output, temp);
	}

	// Clean up

	fgp_vkf_free(temp);

	/**** END OF WRITING, DON'T EVEN DARE ******/


	return 0;
}/* 

Write a function in C that takes as inputs: 
	--> fgp_msg ** inputs 
	--> int size
And as output:
	-->fgp_msg * output

You have a certain number of generic functions at your disposal:
	- int fgp_msg_add(fgp_msg * out, fgp_msg * in1, fgp_msg * in2)
	- int fgp_msg_mul(fgp_msg * out, fgp_msg * in1, fgp_msg * in2)
	- int fgp_msg_copy(fgp_msg * out, fgp_msg * in)
	- int fgp_msg_new(fgp_msg * element)
	- int fgp_msg_free(fgp_msg * element)

To create a new fgp_msg element,
	fgp_msg * element = malloc(sizeof(*element));
	fgp_msg_new(element);
To free a fgp_msg element,
	fgp_msg_free(element);

*/


int func_fgp_msg (fgp_msg * output, fgp_msg ** inputs, bn_t * coefficients, int size)
{

	/***** WRITE YOUR CODE BELOW *****/

	// Temporary variable
	fgp_msg * temp = malloc(sizeof(*temp));
	fgp_msg_new(temp);

	fgp_msg_copy(output, inputs[0]);
	for (int i = 1; i <  size ; ++i)
	{
		fgp_msg_add(output, output, inputs[i]);
	}
	for (int i = 1; i <  size; ++i)
	{
		fgp_msg_mul(temp, inputs[i-1], inputs[i]);
		fgp_msg_const_mult(temp, temp, coefficients[i]);
		fgp_msg_add(output, output, temp);
	}

	// Clean up

	fgp_msg_free(temp);

	/**** END OF WRITING, DON'T EVEN DARE ******/


	return 0;
}