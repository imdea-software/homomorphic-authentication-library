#include "fgp_verify.h"

int fgp_verify(int * check, fgp_private_key * key, char * delta, fgp_vkf * ver_key, fgp_msg * pol, fgp_tag * s)
{
	bn_t mod;
	bn_new(mod);
	g1_get_ord(mod);

	bn_st Wf;
	bn_new_size(&Wf, RELIC_DIGS);

	// Get the a_prime and b_prime

	bn_t a_prime;
	bn_t b_prime;
	bn_new_size(a_prime, RELIC_DIGS);
	bn_new_size(b_prime, RELIC_DIGS);

	if (decouple(a_prime, b_prime, delta, key->K2, KEY_SIZE))
	{
		error_hdl(-1,"Error with the prf on delta");
		return 1;
	}

	// Get Wf
	if (fgp_vkf_eval(&Wf, ver_key, a_prime, b_prime))
	{
		error_hdl(-1,"Problem evaluating Wf");
		return 1;
	}

	gt_t W, gen_t;
	// The generators
	g1_t g;
	g2_t h;

	g1_new(g);
	g2_new(h);

	g1_get_gen(g);
	g2_get_gen(h);

	// gen_t = e(g,h)
	gt_new(gen_t);
	pc_map(gen_t,g,h);
	// W = e(g,h)^Wf
	gt_new(W);
	gt_exp(W,gen_t,&Wf);

	bn_st nu;
	bn_new_size(&nu, RELIC_DIGS);

	if (fgp_msg_eval(&nu, pol, &(key->beta), &(key->alpha)))
	{
		error_hdl(-1,"Error evaluating the fgp_msg to get nu");
		return 1;
	}

	// Do the actual verifications according to the degree of fgp_tag (same as f)
	// Using the following temporary variables to compare to our variables

	bn_st zed_sqr;
	bn_new_size(&zed_sqr, RELIC_DIGS);

	g1_t temp_g1;
	g1_new(temp_g1);

	g2_t temp_g2;
	g2_new(temp_g2);

	gt_t temp_gt1, temp_gt2;
	gt_new(temp_gt1);
	gt_new(temp_gt2);

	// Set the check bit to failure
	*check = 0;	

	switch (s->degree)
	{
		// Here we check: 
		// 			T = g^nu
		//			U = h^nu
		//			e(X,h) = e(g,Y)
		//			W = e(T*X^z, h)
		case 1:
			// T = g^nu
			g1_mul_gen(temp_g1, &nu);
			if (g1_cmp(temp_g1, &(s->T)) == CMP_NE)
				break;

			// U = h^nu
			g2_mul_gen(temp_g2, &nu);
			if (g2_cmp(temp_g2, &(s->U)) == CMP_NE)
				break;
			
			// e(X,h) = e(g,Y)
			pc_map(temp_gt1, g, &(s->Y));
			pc_map(temp_gt2, &(s->X), h);
			if (gt_cmp(temp_gt1, temp_gt2) == CMP_NE)
				break;

			// W = e(T*X^z, h)
			g1_mul(temp_g1, &(s->X), &(key->zed));
			g1_norm(temp_g1, temp_g1);
			g1_add_norm(temp_g1, temp_g1, &(s->T));
			pc_map(temp_gt1, temp_g1, h);
			if (gt_cmp(temp_gt1, W) == CMP_NE)
				break;

			*check = 1;
			break;
		// Here we check: 
		// 			T = e(g,h)^nu
		//			W = T·(X^z)·lambda^(z^2)
		case 2:
			// T = e(g,h)^nu
			gt_exp(temp_gt1, gen_t, &nu);
			
			
			if (gt_cmp(temp_gt1, s->T2) == CMP_NE)
			{
				//printf("Check on T failed\n");
				break;
			}
			
			// W = T·(X^z)·lambda^(z^2)
				// square z
			bn_sqr_mod(&zed_sqr, &(key->zed), mod);
				// X^z
			gt_exp(temp_gt1, s->X2, &(key->zed));
				// lambda^(z^2)
			gt_exp(temp_gt2, s->lambda, &zed_sqr);
				// X^z*lambda^(z^2)
			gt_mul(temp_gt1, temp_gt1, temp_gt2);
				// T·(X^z)·lambda^(z^2)
			gt_mul(temp_gt1, temp_gt1, s->T2);

			if (gt_cmp(temp_gt1, W) == CMP_NE)
			{
				//printf("Check on W failed\n");
				break;
			}
			*check = 1;
			break;
		default:
			error_hdl(-1,"Degree of fgp_tag not accepted");
			return 1;
	};

	// Clean up
	bn_clean(&zed_sqr);
	bn_clean(&nu);
	bn_clean(&Wf);
	bn_clean(a_prime);
	bn_clean(b_prime);
	bn_clean(mod); 

	g1_free(g);
	g2_free(h);
	g1_free(temp_g1);
	g2_free(temp_g2);
	gt_free(temp_gt1);
	gt_free(temp_gt2);
	gt_free(gen_t);
	gt_free(W);
	
	return 0;
}