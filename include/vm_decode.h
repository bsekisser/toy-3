static void vm_step_1_decode_ea_xx_xx(_PASS_VM, _PASS_INST)
{
	EA = PC + IR_V24s;
}

static void vm_step_1_decode_ra_zz_zz(_PASS_VM, _PASS_INST)
{
	RAr = IR_RD;
	RA = RFV(RAr);
}

static void vm_step_1_decode_rd_zz_zz(_PASS_VM, _PASS_INST)
{
	WBd = 1;

	RDr = IR_RD;
}

static void vm_step_1_decode_rdea_ra(_PASS_VM, _PASS_INST, int io)
{
	WBma = 1;
	
	RDr = IR_RD;
	RD = RFV(RDr);
	
	EA = RD + IR_V16s;
	
	vm_step_1_decode_ra_zz_zz(vm, inst);
}

static void vm_step_1_decode_rdrc_zz_zz(_PASS_VM, _PASS_INST)
{
	vm_step_1_decode_rd_zz_zz(vm, inst);

	WBc = 1;

	RCr = IR_RC;
}

static void vm_step_1_decode_rd_ra_zz(_PASS_VM, _PASS_INST)
{
	vm_step_1_decode_rd_zz_zz(vm, inst);

	RAr = IR_RA;
	RA = RFV(RAr);
}

static void vm_step_1_decode_rd_raea(_PASS_VM, _PASS_INST)
{
	vm_step_1_decode_rd_ra_zz(vm, inst);

	EA = RA + IR_V16s;
}

static void vm_step_1_decode_rd_ra_rb(_PASS_VM, _PASS_INST)
{
	vm_step_1_decode_rd_ra_zz(vm, inst);

	RBr = IR_RB;
	RB = RFV(RBr);

	VV = IR_V8;
}

static void vm_step_1_decode_rdrc_ra_rb(_PASS_VM, _PASS_INST)
{
	vm_step_1_decode_rdrc_zz_zz(vm, inst);
}

static void vm_step_1_decode_rd_ra_rb_rc(_PASS_VM, _PASS_INST)
{
	vm_step_1_decode_rdrc_ra_rb(vm, inst);

	RC = RFV(RCr);
}

static void vm_step_1_decode_rd_ra_vv(_PASS_VM, _PASS_INST)
{
	vm_step_1_decode_rd_ra_zz(vm, inst);

	RBr = IR_RB;
	RB = RFV(RBr);

	VV = IR_V16;
}

static void vm_step_1_decode_rd_vv_vv(_PASS_VM, _PASS_INST)
{
	vm_step_1_decode_rd_zz_zz(vm, inst);

	VV = IR_V20;
}

static void vm_step_1_decode_ra_rb_zz(_PASS_VM, _PASS_INST)
{
	vm_step_1_decode_ra_zz_zz(vm, inst);

	RBr = IR_RA;
	RB = RFV(RBr);
}

static void vm_step_1_decode_raea_rb(_PASS_VM, _PASS_INST)
{
	vm_step_1_decode_ra_rb_zz(vm, inst);

	EA = RA + IR_V16s;
}

static void vm_step_1_decode_ra_rb_pcea(_PASS_VM, _PASS_INST)
{
	vm_step_1_decode_ra_rb_zz(vm, inst);

	EA = PC + IR_V16s;
}

/*static void vm_step_1_decode_ra_vv_vv(_PASS_VM, _PASS_INST)
{
	vm_step_1_decode_ra_zz_zz(vm, inst);

	VV = IR_V20;
}*/

static void vm_step_1_decode_xx_xx_xx(_PASS_VM, _PASS_INST)
{
}

#undef INST_ESAC
#define INST_ESAC(_esac, _fn, _action) \
	vm_step_1_decode_##_fn,

typedef void (*decode_fn_t)(_PASS_VM, _PASS_INST);

static decode_fn_t vm_step_1_decode_fn[] = {
	INST_ESAC_LIST
};

#undef INST_ESAC
