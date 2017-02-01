#include "cf13_msg.h"

int cf13_msg_new(cf13_msg * m, cf13_public_key *pk)
{
	fq_ctx_init(m->ctx, fq_ctx_prime(pk->ctx), fq_ctx_degree(pk->ctx), "ctx");
	
	fq_init(m->msg, m->ctx);
	
	return 0;
}


int cf13_msg_free(cf13_msg * m)
{
	fq_clear(m->msg, m->ctx);
	
	fq_ctx_clear(m->ctx);
	
	free(m);

	return 0;
}

int cf13_msg_rand(cf13_msg * m)
{
	if(hal_get_rand_fq(m->msg, m->ctx))
		return 1;
	
	return 0;
}

int cf13_msg_add(cf13_msg * m_out, cf13_msg * m1, cf13_msg * m2)
{
	fq_add(m_out->msg, m1->msg, m2->msg, m1->ctx);
	
	return 0;
}

int cf13_msg_copy(cf13_msg * m_out, cf13_msg * m_in)
{
	fq_set(m_out->msg, m_in->msg, m_in->ctx);
	
	return 0;
}

int cf13_msg_mul(cf13_msg * m_out, cf13_msg * m1, cf13_msg * m2)
{
	fq_mul(m_out->msg, m1->msg, m2->msg, m1->ctx);

	return 0;
}


int cf13_msg_print(cf13_msg * m)
{
	fq_print_pretty(m->msg, m->ctx);
	return 0;
}

int cf13_msg_read(cf13_msg * m, unsigned char * file_name)
{
	return 0;
}

int cf13_msg_write(unsigned char * file_name, cf13_msg * m)
{
	return 0;
}
