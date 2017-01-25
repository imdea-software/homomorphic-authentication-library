#include <fgp.h>

// Applies the function to the fgp_tags
int func_fgp_tag(fgp_tag * output, fgp_tag ** inputs, bn_t * coefficients, int size);

// Applies the function to the fgp_vkf polynomials
int func_fgp_vkf(fgp_vkf * output, fgp_vkf ** inputs, bn_t * coefficients, int size);

// Applies the function to the message polynomials
int func_fgp_msg (fgp_msg * output, fgp_msg ** inputs, bn_t * coefficients, int size);
