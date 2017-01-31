#include "cf13_verify.h"

int cf13_verify(int * check, cf13_private_key * key, cf13_vkf * ver_key, cf13_msg * m, cf13_tag * s)
{
	fq_t y_evald, y0;
	fq_init(y_evald, key->ctx);
	fq_init(y0, key->ctx);
	
	//compute y(x)
	
	fq_poly_evaluate_fq(y_evald, s->y, key->x, s->ctx);
	
	//get y(0)
	fq_poly_get_coeff(y0, s->y, 0, s->ctx);
	
	/** DEBUGGING
	printf("\ny0: ");
	fq_print_pretty(y0, s->ctx);
	printf("\ny(x): ");
	fq_print_pretty(y_evald, s->ctx);
	printf("\nVKf: ");
	cf13_vkf_print(ver_key);
	*/
	
	//check y(x)=f(r1..rn)=ver_key->r & y(0)=m
	*check = 0;
	if(fq_equal(y_evald, ver_key->r, key->ctx) && fq_equal(y0, m->msg, key->ctx))
		*check = 1;

	// Clean up
	fq_clear(y_evald, key->ctx);
	fq_clear(y0, key->ctx);
	
	return 0;
}
