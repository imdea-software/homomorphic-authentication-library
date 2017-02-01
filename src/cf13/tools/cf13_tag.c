#include "cf13_tag.h"


int cf13_tag_new(cf13_tag *s, cf13_private_key *sk)
{
	fq_ctx_init(s->ctx, fq_ctx_prime(sk->ctx), fq_ctx_degree(sk->ctx), "ctx");
	
	fq_poly_init(s->y, s->ctx);
	
	return 0;
}

int cf13_tag_free(cf13_tag *s)
{
	fq_poly_clear(s->y, s->ctx);
	fq_ctx_clear(s->ctx);
	free(s);
	
	return 0;
}

int cf13_tag_copy(cf13_tag * s_out, cf13_tag * s_in)
{
	fq_poly_set(s_out->y, s_in->y, s_in->ctx);
	
	return 0;
}

int cf13_tag_const_mult(cf13_tag *s_out, cf13_tag *s_in, cf13_msg * c)
{
	fq_poly_scalar_mul_fq(s_out->y, s_in->y, c->msg, s_in->ctx);
	
	return 0;
}

int cf13_tag_const_add(cf13_tag *s_out, cf13_tag *s_in, cf13_msg * c)
{
	fq_poly_t tmp;
	fq_poly_init(tmp, s_in->ctx);
	fq_poly_set_fq(tmp, c->msg, s_in->ctx);
	
	fq_poly_add(s_out->y, s_in->y, tmp, s_in->ctx);
	fq_poly_clear(tmp, s_in->ctx);
	
	return 0;
}

int cf13_tag_mul(cf13_tag * s, cf13_tag * s1, cf13_tag * s2)
{
	fq_poly_mul(s->y, s1->y, s2->y, s1->ctx);
	
	return 0;
}

int cf13_tag_add(cf13_tag * s, cf13_tag * s1, cf13_tag * s2)
{
	fq_poly_add(s->y, s1->y, s2->y, s1->ctx);
	
	return 0;
}



int cf13_tag_print(cf13_tag *s)
{
	fq_poly_print_pretty(s->y, "X", s->ctx);

	return 0;
}

int cf13_tag_write(unsigned char * file, cf13_tag * s)
{	
	FILE * f = fopen(file , "w+");
	if (f == NULL)
	{
		error_hdl(-1,"error opening the file");
		return 1;
	}
	
	fq_poly_fprint(f, s->y, s->ctx);

	fclose(f);

	return 0;
}

int cf13_tag_read(cf13_tag * s, unsigned char * file)
{
	//not implemented yet
	
	return 0;
}


