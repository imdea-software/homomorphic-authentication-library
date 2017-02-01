#include "cf13_verification_keys.h"

int cf13_vkf_new(cf13_vkf * r, cf13_private_key *sk)
{
	fq_ctx_init(r->ctx, fq_ctx_prime(sk->ctx), fq_ctx_degree(sk->ctx), "ctx");
	
	
	fq_init(r->r, r->ctx);

	return 0;
}

int cf13_vkf_copy(cf13_vkf *r_out, cf13_vkf *r_in)
{
	fq_set(r_out->r, r_in->r, r_in->ctx);

	return 0;
}

int cf13_vkf_free(cf13_vkf *r)
{
	fq_clear(r->r, r->ctx);
	
	fq_ctx_clear(r->ctx);

	free(r);
	return 0;
}


int cf13_vkf_const_mult(cf13_vkf * r_out, cf13_vkf * r_in, cf13_msg * constant)
{	
	fq_mul(r_out->r, r_in->r, constant->msg, r_in->ctx);
	
	return 0;
}



int cf13_vkf_add(cf13_vkf * r_out, cf13_vkf * r1, cf13_vkf * r2)
{
	fq_add(r_out->r, r1->r, r2->r, r1->ctx);
	
	return 0;
}

int cf13_vkf_mul(cf13_vkf *r_out, cf13_vkf *r1, cf13_vkf *r2)
{
	fq_mul(r_out->r, r1->r, r2->r, r1->ctx);

	return 0;
}

int cf13_vkf_set(cf13_vkf * r, cf13_private_key * key, char * L)
{
	if (cf13_prf(&(r->r), &(r->ctx), L, key->K, key->size))
	{
		error_hdl(-1,"Error with the prf on L");
		return 1;
	}
	
	return 0;
}

int cf13_vkf_const_add(cf13_vkf * r_out, cf13_vkf * r_in, cf13_msg * constant)
{
	fq_add(r_out->r, r_in->r, constant->msg, r_in->ctx);
	
	return 0;
}


int cf13_vkf_print(cf13_vkf *r)
{
	fq_print_pretty(r->r, r->ctx);
	
	return 0;
}


int cf13_vkf_write(unsigned char * file, cf13_vkf * r)
{
	FILE *f = fopen(file, "w+");
	if (f==NULL)
	{
		error_hdl(-1,"Error opening the file");
		return 1;
	}

	fq_fprint(f, r->r, r->ctx);
	
	fclose(f);
	return 0;
}


int cf13_vkf_read(cf13_vkf * r, unsigned char * file)
{
	//not implemented yet
	
	return 0;
}
