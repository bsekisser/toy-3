static void alu_flag_fn_add(_PASS_VM, uint32_t rd_v, uint32_t s1_v, uint32_t s2_v)
{
	const uint32_t xvec = (s1_v ^ s2_v);
	const uint32_t ovec = (s1_v ^ rd_v) & ~xvec;

	PSR &= ~PSR_CNVZ;

	PSR |= BMOV(rd_v, 31, PSR_BIT_NF);
	PSR |= ((rd_v == 0) ? PSR_ZF : 0);

	PSR |= BMOV((xvec ^ ovec ^ rd_v), 31, PSR_BIT_CF);
	PSR |= BMOV(ovec, 31, PSR_BIT_OVF);
}

static void alu_flag_fn_nz(_PASS_VM, uint32_t rd_v)
{
	PSR &= ~(PSR_NF | PSR_ZF);
	
	PSR |= BMOV(rd_v, 31, PSR_BIT_NF);
	PSR |= ((rd_v == 0) ? PSR_ZF : 0);
}

static void alu_flag_fn_sub(_PASS_VM, uint32_t rd_v, uint32_t s1_v, uint32_t s2_v)
{
	alu_flag_fn_add(vm, rd_v, s1_v, ~s2_v + 1);
}
