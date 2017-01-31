#include "cf13_authenticate.h"

int cf13_authenticate(cf13_tag *s, cf13_private_key * key, char * L, cf13_msg * m)
{
	// Initialize variables
	fq_t r, y1;
	fq_init(r, key->ctx);
	fq_init(y1, key->ctx);
	
	
	//compute r=F_K(L)
	if(cf13_prf(r, key->ctx, L, key->K, key->size))
	{
		error_hdl(-1,"Error with the prf on L");
		return 1;
	}
	
	//compute y1=(r-m)/x
	
	//y1=r-m
	fq_sub(y1, r, m->msg, key->ctx);
	//y1=y1/x
	fq_div(y1, y1, key->x, key->ctx);
	
	//set y = y0 + y1*X
	fq_poly_set_coeff(s->y, 0, m->msg, key->ctx);
	fq_poly_set_coeff(s->y, 1, y1, key->ctx);
	
	
	// Clean up
	fq_clear(r, key->ctx);
	fq_clear(y1, key->ctx);
	
	return 0;
}
