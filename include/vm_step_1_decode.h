#define TRACE_DECODE_LEVEL 1

#define TRACE_DECODE(_level, _f, args...) \
	({ \
		if(_TRACE_DECODE && (TRACE_DECODE_LEVEL >= _level)) \
		{ \
			TRACE(_f, ##args); \
		}\
	});


#define _BMASR(_data, _shift) \
	({ \
		int32_t _rval = _data & _BM(_shift); \
		_data = _asr(_data, _shift); \
		_rval; \
	})

/* **** */

static void _decode__a(_PASS_VM, _PASS_INST, int32_t* opcode, uint8_t *bits)
{
	rR(A) = _BMASR(*opcode, IR_REG_BITS);
	
	*bits += IR_REG_BITS;

	TRACE_DECODE(3, "rA = 0x%02x, opcode = 0x%08x, bits = 0x%02x, left = 0x%02x",
		rR(A), *opcode, *bits, 32 - *bits);
	
	assert(*bits <= 32);
}

static void _decode__b(_PASS_VM, _PASS_INST, int32_t* opcode, uint8_t *bits)
{
	rR(B) = _BMASR(*opcode, IR_REG_BITS);

	*bits += IR_REG_BITS;

	TRACE_DECODE(3, "rB = 0x%02x, opcode = 0x%08x, bits = 0x%02x, left = 0x%02x",
		rR(B), *opcode, *bits, 32 - *bits);

	assert(*bits <= 32);
}

static void _decode__c(_PASS_VM, _PASS_INST, int32_t* opcode, uint8_t *bits)
{
	rR(C) = _BMASR(*opcode, IR_REG_BITS);

	*bits += IR_REG_BITS;

	TRACE_DECODE(3, "rC = 0x%02x, opcode = 0x%08x, bits = 0x%02x, left = 0x%02x",
		rR(C), *opcode, *bits, 32 - *bits);

	assert(*bits <= 32);
}

static void _decode__d(_PASS_VM, _PASS_INST, int32_t* opcode, uint8_t *bits)
{
	rR(D) = _BMASR(*opcode, IR_REG_BITS);

	*bits += IR_REG_BITS;

	TRACE_DECODE(3, "rD = 0x%02x, opcode = 0x%08x, bits = 0x%02x, left = 0x%02x",
		rR(D), *opcode, *bits, 32 - *bits);

	assert(*bits <= 32);
}

static void _decode__op(_PASS_VM, _PASS_INST, int32_t* opcode, uint8_t *bits)
{
	*opcode = _asr(IR, IR_OP_BITS);

	*bits += IR_OP_BITS;

	TRACE_DECODE(3, "opcode = 0x%08x, bits = 0x%02x, left = 0x%02x",
		*opcode, *bits, 32 - *bits);

	assert(*bits <= 32);
}

/* **** */

static void _decode__ra(_PASS_VM, _PASS_INST, int32_t* opcode, uint8_t *bits)
{
	_decode__a(vm, inst, opcode, bits);
	vR(A) = rR(A) ? GPR(rR(A)) : 0;
}

static void _decode__rb(_PASS_VM, _PASS_INST, int32_t* opcode, uint8_t *bits)
{
	_decode__b(vm, inst, opcode, bits);
	vR(B) = rR(B) ? GPR(rR(B)) : 0;
}

static void _decode__rc(_PASS_VM, _PASS_INST, int32_t* opcode, uint8_t *bits)
{
	_decode__c(vm, inst, opcode, bits);
	vR(C) = rR(C) ? GPR(rR(C)) : 0;
}

/* **** */

static void _decode_op_ra(_PASS_VM, _PASS_INST, int32_t* opcode, uint8_t *bits)
{
	_decode__op(vm, inst, opcode, bits);
	_decode__ra(vm, inst, opcode, bits);
}

static void _decode_op_ra_rb(_PASS_VM, _PASS_INST, int32_t* opcode, uint8_t *bits)
{
	_decode_op_ra(vm, inst, opcode, bits);
	_decode__rb(vm, inst, opcode, bits);
}

static void _decode_op_ra_rb_rc(_PASS_VM, _PASS_INST, int32_t* opcode, uint8_t *bits)
{
	_decode_op_ra_rb(vm, inst, opcode, bits);
	_decode__rc(vm, inst, opcode, bits);
}

/* **** */

static void _decode_op_rd(_PASS_VM, _PASS_INST, int32_t* opcode, uint8_t *bits)
{
	_decode__op(vm, inst, opcode, bits);
	_decode__d(vm, inst, opcode, bits);
}

static void _decode_op_rd_ra(_PASS_VM, _PASS_INST, int32_t* opcode, uint8_t *bits)
{
	_decode_op_rd(vm, inst, opcode, bits);
	_decode__ra(vm, inst, opcode, bits);
}

static void _decode_op_rd_ra_rb(_PASS_VM, _PASS_INST, int32_t* opcode, uint8_t *bits)
{
	_decode_op_rd_ra(vm, inst, opcode, bits);
	_decode__rb(vm, inst, opcode, bits);
}

/* **** */

static void vm_step_1_decode_nop(_PASS_VM, _PASS_INST)
{
	TAILCALL_NEXT();
}

static void vm_step_1_decode_type_pcea(_PASS_VM, _PASS_INST)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode__op(vm, inst, &opcode, &bits);

	EA = PC + (opcode << 2);
		
	TRACE_DECODE(1, "0x%08x[rPC] = 0x%08x", opcode, EA);

	TAILCALL_NEXT();
}

static void vm_step_1_decode_type_ra(_PASS_VM, _PASS_INST)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode_op_ra(vm, inst, &opcode, &bits);

	TRACE_DECODE(1, "rA = 0x%02x", rR(A));
	
	TAILCALL_NEXT();
}

static void vm_step_1_decode_type_ra_b(_PASS_VM, _PASS_INST)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode_op_ra(vm, inst, &opcode, &bits);
	_decode__b(vm, inst, &opcode, &bits);

	TRACE_DECODE(1, "rA = 0x%02x, B = 0x%02x", rR(A), rR(B));

	TAILCALL_NEXT();
}

static void vm_step_1_decode_type_ra_rb(_PASS_VM, _PASS_INST)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode_op_ra_rb(vm, inst, &opcode, &bits);

	TRACE_DECODE(1, "rA = 0x%02x, rB = 0x%02x", rR(A), rR(B));

	VV = opcode;

	TRACE_DECODE(1, "VV = 0x%08x", VV);

	TAILCALL_NEXT();
}

static void vm_step_1_decode_type_ra_rb_pcea(_PASS_VM, _PASS_INST)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode_op_ra_rb(vm, inst, &opcode, &bits);

	TRACE_DECODE(1, "rA = 0x%02x, rB = 0x%02x", rR(A), rR(B));

	EA = PC + (opcode << 2);

	TRACE_DECODE(1, "EA = 0x%08x", EA);

	TAILCALL_NEXT();
}

static void vm_step_1_decode_type_rd(_PASS_VM, _PASS_INST)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode_op_rd(vm, inst, &opcode, &bits);

	TRACE_DECODE(1, "rD = 0x%02x", rR(D));

	TAILCALL_NEXT();
}

static void vm_step_1_decode_type_rd_a(_PASS_VM, _PASS_INST)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode_op_rd(vm, inst, &opcode, &bits);
	_decode__a(vm, inst, &opcode, &bits);

	TRACE_DECODE(1, "rD = 0x%02x, A = 0x%02x", rR(D), rR(A));

	TAILCALL_NEXT();
}

static void vm_step_1_decode_type_rd_ra(_PASS_VM, _PASS_INST)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode_op_rd_ra(vm, inst, &opcode, &bits);

	TRACE_DECODE(1, "rD = 0x%02x, rA = 0x%02x", rR(D), rR(A));

	TAILCALL_NEXT();
}

static void vm_step_1_decode_type_rd_ra_pcea(_PASS_VM, _PASS_INST)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode_op_rd_ra(vm, inst, &opcode, &bits);

	TRACE_DECODE(1, "rD = 0x%02x, rA = 0x%02x", rR(D), rR(A));

	EA = PC + (opcode << 2);

	TRACE_DECODE(1, "EA = 0x%08x", EA);

	TAILCALL_NEXT();
}

static void vm_step_1_decode_type_rd_ra_b_c(_PASS_VM, _PASS_INST)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode_op_rd_ra(vm, inst, &opcode, &bits);
	_decode__b(vm, inst, &opcode, &bits);
	_decode__c(vm, inst, &opcode, &bits);

	TRACE_DECODE(1, "rD = 0x%02x, rA = 0x%02x, rB = 0x%02x, rC = 0x%02x",
		rR(D), rR(A), rR(B), rR(C));

	VV = opcode;

	TRACE_DECODE(1, "VV = 0x%08x", VV);

	TAILCALL_NEXT();
}

static void vm_step_1_decode_type_rd_ra_i(_PASS_VM, _PASS_INST)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode_op_rd_ra(vm, inst, &opcode, &bits); /* 6+5+5 = 16, 16 */

	TRACE_DECODE(1, "rD = 0x%02x, rA = 0x%02x", rR(D), rR(A));

	VV = opcode;

	TRACE_DECODE(1, "VV = 0x%08x", VV);

	TAILCALL_NEXT();
}

/*static void vm_step_1_decode_type_rd_ra_u(_PASS_VM, _PASS_INST)
{
	TRACE_DECODE();
	
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode_op_rd_ra(vm, inst, &opcode, &bits);

	VV = opcode & 0xffff;

	TAILCALL_NEXT();
}*/

static void vm_step_1_decode_type_rd_ra_rb(_PASS_VM, _PASS_INST)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode_op_rd_ra_rb(vm, inst, &opcode, &bits);

	TRACE_DECODE(1, "rD = 0x%02x, rA = 0x%02x, rB = 0x%02x",
		rR(D), rR(A), rR(B));

	VV = opcode;

	TRACE_DECODE(1, "VV = 0x%08x", VV);

	TAILCALL_NEXT();
}

static void vm_step_1_decode_type_rd_ra_rb_rc(_PASS_VM, _PASS_INST)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode_op_rd_ra_rb(vm, inst, &opcode, &bits);
	_decode__rc(vm, inst, &opcode, &bits);

	TRACE_DECODE(1, "rD = 0x%02x, rA = 0x%02x, rB = 0x%02x, rC = 0x%02x",
		rR(D), rR(A), rR(B), rR(C));

	VV = opcode;

	TRACE_DECODE(1, "VV = 0x%08x", VV);

	TAILCALL_NEXT();
}

static void vm_step_1_decode_type_rd_ra_rbsa(_PASS_VM, _PASS_INST)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode_op_rd_ra_rb(vm, inst, &opcode, &bits);

	TRACE_DECODE(1, "rD = 0x%02x, rA = 0x%02x, rB = 0x%02x", rR(D), rR(A), rR(B));

	SA = rR(B) ? vR(B) : opcode;

	TRACE_DECODE(1, "SA = 0x%08x", SA);

	TAILCALL_NEXT();
}

static void vm_step_1_decode_type_rd_raea(_PASS_VM, _PASS_INST)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode_op_rd_ra(vm, inst, &opcode, &bits);

	TRACE_DECODE(1, "rD = 0x%02x, rA = 0x%02x", rR(D), rR(A));

	if(0)
		EA = rR(A) ? vR(A) : SP;
	else
		EA = vR(A);

	EA += opcode;

	TRACE_DECODE(1, "0x%08x[%s] = 0x%08x", opcode, rR(A) ? "rA" : "rSP", EA);

	TAILCALL_NEXT();
}

static void vm_step_1_decode_type_rdea_ra(_PASS_VM, _PASS_INST)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode_op_rd_ra(vm, inst, &opcode, &bits);

	TRACE_DECODE(1, "rD = 0x%02x, rA = 0x%02x", rR(D), rR(A));

	vR(D) = rR(D) ? GPR(rR(D)) : 0;
	
	if(0)
		EA = rR(D) ? vR(D) : SP;
	else
		EA = vR(D);

	EA += opcode;

	TRACE_DECODE(1, "0x%08x[%s] = 0x%08x", opcode, rR(D) ? "rD" : "rSP", EA);

	TAILCALL_NEXT();
}

static void vm_step_1_decode_type_u(_PASS_VM, _PASS_INST)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode__op(vm, inst, &opcode, &bits);

	VV = opcode;
		
	TAILCALL_NEXT();
}
