#include "fgp_authenticate.h"

int fgp_authenticate(fgp_tag *s, fgp_private_key * key, char * delta, char * L, fgp_msg * pol)	
{

	bn_t mod;
	bn_new(mod);
	g1_get_ord(mod);

	// Initialize variables
	bn_t a_prime;
	bn_t b_prime;
	bn_new_size(a_prime, RELIC_DIGS);
	bn_new_size(b_prime, RELIC_DIGS);

	/*
	// Check the input variables
	if (pol->xy2degree != -1)
	{
		error_hdl(-1,"Wrong degree for the fgp_msg. Has to be of degree 1 or less in Y");
		return 1;
	}
	*/

	// Create the bilinear polynomial through the prf

	fgp_vkf * r = malloc(sizeof(*r));
	// Create the polynomial
	fgp_vkf_new(r);



	// Set the fgp_vkf
	fgp_vkf_set(r, key, L);

	// Store the result of the fgp_msg evaluation in the "nu" variable


	bn_st nu;
	bn_new_size(&nu, RELIC_DIGS);

	if (fgp_msg_eval(&nu, pol, &(key->beta), &(key->alpha)))
	{
		error_hdl(-1,"Error with the alpha/beta evaluation");
		return 1;
	}

	// Get the a_prime and b_prime

	if (decouple(a_prime, b_prime, delta, key->K2, KEY_SIZE))
	{
		error_hdl(-1,"Error with the prf on delta");
		return 1;
	}

	// Compute m2
	bn_st m2;
	bn_new_size(&m2, RELIC_DIGS);

	if (fgp_vkf_eval(&m2, r, a_prime, b_prime))
	{
		error_hdl(-1,"Error with the bilinear fgp_msg evaluation");
		return 1;
	}
	// Compute inv_zed
	bn_t inv_zed;
	bn_new_size(inv_zed, RELIC_DIGS);


	if (inverse(inv_zed, &(key->zed), mod, RELIC_DIGS))
	{
		error_hdl(-1,"Error with the inverse of zed");
		return 1;
	}

	bn_sub_mod(&m2, &m2, &nu, mod);					// m2 = m2 - nu
	bn_mul_mod(&m2, &m2, inv_zed, mod);		// Divide by zed

	// Exponentiations
	/*
	g1_mul_fix(&(s->T), ctx->preComp1, &nu);
	g2_mul_fix(&(s->U),ctx->preComp2, &nu);
	g1_mul_fix(&(s->X),ctx->preComp1, &m2);
	g2_mul_fix(&(s->Y),ctx->preComp2, &m2);
	*/
	g1_mul_gen(&(s->T),&nu);
	g2_mul_gen(&(s->U),&nu);
	g1_mul_gen(&(s->X),&m2);
	g2_mul_gen(&(s->Y),&m2);
	
	gt_set_unity(s->lambda);


	// Clean up
	if (fgp_vkf_free(r))
	{
	 	error_hdl(-1,"Problem cleaning the fgp_msg fgp_vkf");
	 	return 1;
	} 

	bn_clean(a_prime);
	bn_clean(b_prime);
	bn_clean(&nu);
	bn_clean(&m2);
	bn_clean(inv_zed);
	bn_clean(mod);

	return 0;
}