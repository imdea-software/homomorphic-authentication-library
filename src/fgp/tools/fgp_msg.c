#include "fgp_msg.h"

int fgp_msg_new(fgp_msg * pol)
{
	fmpz_t mod;
	mod_set_fmpz(mod);

	fq_ctx_init(pol->ctx, mod, 1, "ctx");

	fq_poly_init(pol->xpol, pol->ctx);
	fq_poly_init(pol->xypol, pol->ctx);
	fq_poly_init(pol->xy2pol, pol->ctx);

	fmpz_clear(mod);

	return 0;
}

int fgp_msg_eval(bn_st * output, fgp_msg * pol, bn_st * beta, bn_st * alpha)
{
	fq_t a, b, out, temp;
	fq_init(a, pol->ctx);
	fq_init(b, pol->ctx);
	fq_init(out, pol->ctx);
	fq_init(temp, pol->ctx);

	bn2fq(a, alpha, pol->ctx);
	bn2fq(b, beta, pol->ctx);

	// X
	fq_poly_evaluate_fq(out, pol->xpol, b, pol->ctx);

	// XY
	fq_poly_evaluate_fq(temp, pol->xypol, b, pol->ctx);
	fq_mul(temp, temp, a, pol->ctx);
	fq_add(out, out, temp, pol->ctx);

	// XY^2
	fq_poly_evaluate_fq(temp, pol->xy2pol, b, pol->ctx);
	fq_mul(temp, temp, a, pol->ctx);
	fq_mul(temp, temp, a, pol->ctx);
	fq_add(out, out, temp, pol->ctx);

	fq2bn(output, out, pol->ctx);

	// Clean up
	fq_clear(temp, pol->ctx);
	fq_clear(out, pol->ctx);
	fq_clear(a, pol->ctx);
	fq_clear(b, pol->ctx);
	return 0;
}

int fgp_msg_clean(fgp_msg * pol)
{
	fq_poly_realloc(pol->xpol, 0, pol->ctx);
	fq_poly_realloc(pol->xypol, 0, pol->ctx);
	fq_poly_realloc(pol->xy2pol, 0, pol->ctx);
	return 0;
}

int fgp_msg_free(fgp_msg * pol)
{
	fq_poly_clear(pol->xpol, pol->ctx);
	fq_poly_clear(pol->xypol, pol->ctx);
	fq_poly_clear(pol->xy2pol, pol->ctx);

	fq_ctx_clear(pol->ctx);
	
	free(pol);

	return 0;
}

int fgp_msg_rand(fgp_msg * pol, int xdegree, int ydegree)
{
	//fgp_msg_clean(pol);

	flint_rand_t state;
	flint_randinit(state);

	switch(ydegree)
	{
		case 2:
			fq_poly_randtest(pol->xy2pol, state, xdegree, pol->ctx);
			fq_poly_randtest(pol->xypol, state, xdegree, pol->ctx);
			fq_poly_randtest(pol->xpol, state, xdegree, pol->ctx);
			break;
		case 1:
			fq_poly_randtest(pol->xypol, state, xdegree, pol->ctx);
			fq_poly_randtest(pol->xpol, state, xdegree, pol->ctx);
			break;
		case 0:
			fq_poly_randtest(pol->xpol, state, xdegree, pol->ctx);
			break;
		default:
			error_hdl(-1,"Y degree not accepted");
			return 1;
	};

	flint_randclear(state);
	return 0;
}

int fgp_msg_add(fgp_msg * pol_out, fgp_msg * pol1, fgp_msg * pol2)
{
	fq_poly_add(pol_out->xpol, pol1->xpol, pol2->xpol, pol1->ctx);
	fq_poly_add(pol_out->xypol, pol1->xypol, pol2->xypol, pol1->ctx);
	fq_poly_add(pol_out->xy2pol, pol1->xy2pol, pol2->xy2pol, pol1->ctx);

	return 0;
}

int fgp_msg_copy(fgp_msg * pol_out, fgp_msg * pol_in)
{
	fq_poly_set(pol_out->xpol, pol_in->xpol, pol_in->ctx);
	fq_poly_set(pol_out->xypol, pol_in->xypol, pol_in->ctx);
	fq_poly_set(pol_out->xy2pol, pol_in->xy2pol, pol_in->ctx);

	return 0;
}

int fgp_msg_mul(fgp_msg * pol_out, fgp_msg * pol1, fgp_msg * pol2)
{
	if (fq_poly_length(pol1->xy2pol, pol1->ctx) != 0 || fq_poly_length(pol2->xy2pol, pol2->ctx) != 0)
	{
		error_hdl(-1, "The input polynomials are not both of degree 2 in y");
		return 1;
	}

	fq_poly_t a, b, tempx, tempxy, tempxy2;
	fq_poly_init(tempx, pol1->ctx);
	fq_poly_init(tempxy, pol1->ctx);
	fq_poly_init(tempxy2, pol1->ctx);
	fq_poly_init(a, pol1->ctx);
	fq_poly_init(b, pol1->ctx);

	fq_poly_mul(a, pol1->xpol, pol2->xypol, pol1->ctx);
	fq_poly_mul(b, pol1->xypol, pol2->xpol, pol1->ctx);
	fq_poly_add(tempxy, a, b, pol1->ctx);

	fq_poly_mul(tempx, pol1->xpol, pol2->xpol, pol1->ctx);
	fq_poly_mul(tempxy2, pol1->xypol, pol2->xypol, pol1->ctx);

	fq_poly_set(pol_out->xpol, tempx, pol1->ctx);
	fq_poly_set(pol_out->xypol, tempxy, pol1->ctx);
	fq_poly_set(pol_out->xy2pol, tempxy2, pol1->ctx);

	fq_poly_clear(a, pol1->ctx);
	fq_poly_clear(b, pol1->ctx);
	fq_poly_clear(tempx, pol1->ctx);
	fq_poly_clear(tempxy, pol1->ctx);
	fq_poly_clear(tempxy2, pol1->ctx);

	return 0;
}

int fgp_msg_const_mult(fgp_msg * pol_out, fgp_msg * pol_in, bn_st * constant)
{
	fq_t c;
	fq_init(c, pol_in->ctx);

	bn2fq(c, constant, pol_in->ctx);

	fq_poly_scalar_mul_fq(pol_out->xpol, pol_in->xpol, c, pol_in->ctx);
	fq_poly_scalar_mul_fq(pol_out->xypol, pol_in->xypol, c, pol_in->ctx);
	fq_poly_scalar_mul_fq(pol_out->xy2pol, pol_in->xy2pol, c, pol_in->ctx);

	fq_clear(c, pol_in->ctx);

	return 0;
}

int fgp_msg_const_add(fgp_msg * pol_out, fgp_msg * pol_in, bn_st * constant)
{
	fq_t c, coef;
	fq_init(c, pol_in->ctx);
	fq_init(coef, pol_in->ctx);

	bn2fq(c, constant, pol_in->ctx);

	fq_poly_get_coeff(coef, pol_in->xpol, 0, pol_in->ctx);
	fq_add(coef, coef, c, pol_in->ctx);

	fq_poly_set_coeff(pol_in->xpol, 0, coef, pol_in->ctx);

	fq_clear(c, pol_in->ctx);
	fq_clear(coef, pol_in->ctx);

	return 0;
}

int fgp_msg_const_sub(fgp_msg * pol_out, fgp_msg * pol_in, bn_st * constant)
{
	fq_t c, coef;
	fq_init(c, pol_in->ctx);
	fq_init(coef, pol_in->ctx);

	bn2fq(c, constant, pol_in->ctx);

	fq_poly_get_coeff(coef, pol_in->xpol, 0, pol_in->ctx);
	fq_sub(coef, coef, c, pol_in->ctx);

	fq_poly_set_coeff(pol_in->xpol, 0, coef, pol_in->ctx);

	fq_clear(c, pol_in->ctx);
	fq_clear(coef, pol_in->ctx);

	return 0;
}

int fgp_msg_incr(fgp_msg * pol)
{
	fq_t unity;
	fq_init(unity, pol->ctx);
	fq_set_ui(unity, 1, pol->ctx);

	fgp_msg * temp = malloc(sizeof(*temp));
	fgp_msg_new(temp);

	fq_poly_fit_length(temp->xpol, 2, pol->ctx);
	fq_poly_set_coeff(temp->xpol, 1, unity, pol->ctx);

	fgp_msg_mul(pol, temp, pol);

	fgp_msg_free(temp);
	fq_clear(unity, pol->ctx);
	return 0;
}

int fgp_msg_print(fgp_msg * pol)
{
	return 0;
}

int fgp_msg_read(fgp_msg * pol, unsigned char * file_name)
{
	return 0;
}

int fgp_msg_write(unsigned char * file_name, fgp_msg * pol)
{
	return 0;
}

