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

#define _decode__rr(_rrx, _vm, _opcode, _bits) __decode__rr(_vm, _opcode, _bits, &rR(_rrx), #_rrx)
static void __decode__rr(vm_p vm, int32_t* opcode, uint8_t* bits, uint8_t* rr, const char* rrs)
{
	*rr = _BMASR(*opcode, IR_REG_BITS);

	*bits += IR_REG_BITS;

	TRACE_DECODE(3, "r%s = 0x%02x, opcode = 0x%08x, bits = 0x%02x, left = 0x%02x",
		rrs, *rr, *opcode, *bits, 32 - *bits);

	assert(*bits <= 32);
}

static void _decode__op(vm_p vm, int32_t* opcode, uint8_t *bits)
{
	*opcode = _asr(IR, IR_OP_BITS);

	*bits += IR_OP_BITS;

	TRACE_DECODE(3, "opcode = 0x%08x, bits = 0x%02x, left = 0x%02x",
		*opcode, *bits, 32 - *bits);

	assert(*bits <= 32);
}

/* **** */

#define _decode__vr(_rrx, _vm, _opcode, _bits) \
	({ \
		_decode__rr(_rrx, _vm, _opcode, _bits); \
		vR(_rrx) = rR(_rrx) ? GPR(rR(_rrx)) : 0; \
	})

/* **** */

static void _decode_op_vra(vm_p vm, int32_t* opcode, uint8_t *bits)
{
	_decode__op(vm, opcode, bits);
	_decode__vr(A, vm, opcode, bits);
}

static void _decode_op_vra_vrb(vm_p vm, int32_t* opcode, uint8_t *bits)
{
	_decode_op_vra(vm, opcode, bits);
	_decode__vr(B, vm, opcode, bits);
}

/* **** */

static void _decode_op_rd(vm_p vm, int32_t* opcode, uint8_t *bits)
{
	_decode__op(vm, opcode, bits);
	_decode__rr(D, vm, opcode, bits);
}

static void _decode_op_rd_vra(vm_p vm, int32_t* opcode, uint8_t *bits)
{
	_decode_op_rd(vm, opcode, bits);
	_decode__vr(A, vm, opcode, bits);
}

static void _decode_op_rd_vra_vrb(vm_p vm, int32_t* opcode, uint8_t *bits)
{
	_decode_op_rd_vra(vm, opcode, bits);
	_decode__vr(B, vm, opcode, bits);
}

/* **** */

static void vm_step_1_decode_nop(vm_p vm)
{
	TAILCALL_NEXT();
}

#define vm_step_1_decode_nop_nop \
	vm_step_1_decode_nop

/* **** */

static void vm_step_1_decode_type_i_pcea(vm_p vm)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode__op(vm, &opcode, &bits);

	EA = PC + (opcode << 2);
		
	TRACE_DECODE(1, "0x%08x[rPC] = 0x%08x", opcode, EA);

	TAILCALL_NEXT();
}

static void vm_step_1_decode_type_i_rd_vra(vm_p vm)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode_op_rd_vra(vm, &opcode, &bits); /* 6+5+5 = 16, 16 */

	TRACE_DECODE(1, "rD = 0x%02x, rA = 0x%02x", rR(D), rR(A));

	VV = opcode;

	TRACE_DECODE(1, "VV = 0x%08x", VV);

	TAILCALL_NEXT();
}

static void vm_step_1_decode_type_i_rd_vra_pcea(vm_p vm)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode_op_rd_vra(vm, &opcode, &bits);

	TRACE_DECODE(1, "rD = 0x%02x, rA = 0x%02x", rR(D), rR(A));

	EA = PC + (opcode << 2);

	TRACE_DECODE(1, "EA = 0x%08x", EA);

	TAILCALL_NEXT();
}

static void vm_step_1_decode_type_i_rd_vraea(vm_p vm)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode_op_rd_vra(vm, &opcode, &bits);

	TRACE_DECODE(1, "rD = 0x%02x, rA = 0x%02x", rR(D), rR(A));

	EA = vR(A) + opcode;	/* !!! byte offset */

	TRACE_DECODE(1, "0x%08x[%s] = 0x%08x", opcode, rR(A) ? "rA" : "rSP", EA);

	TAILCALL_NEXT();
}

static void vm_step_1_decode_type_i_vra_vrb_pcea(vm_p vm)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode_op_vra_vrb(vm, &opcode, &bits);

	TRACE_DECODE(1, "rA = 0x%02x, rB = 0x%02x", rR(A), rR(B));

	EA = PC + (opcode << 2);

	TRACE_DECODE(1, "EA = 0x%08x", EA);

	TAILCALL_NEXT();
}

static void vm_step_1_decode_type_i_vrdea_vra(vm_p vm)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode_op_rd_vra(vm, &opcode, &bits);

	TRACE_DECODE(1, "rD = 0x%02x, rA = 0x%02x", rR(D), rR(A));

	vR(D) = rR(D) ? GPR(rR(D)) : 0;
	
	EA = vR(D) + opcode;	/* !!! byte offset */

	TRACE_DECODE(1, "0x%08x[%s] = 0x%08x", opcode, rR(D) ? "rD" : "rSP", EA);

	TAILCALL_NEXT();
}

/* **** */

static void vm_step_1_decode_type_r_rd(vm_p vm)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode_op_rd(vm, &opcode, &bits);

	TRACE_DECODE(1, "rD = 0x%02x", rR(D));

	TAILCALL_NEXT();
}

static void vm_step_1_decode_type_r_rd_ra(vm_p vm)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode_op_rd(vm, &opcode, &bits);
	_decode__rr(A, vm, &opcode, &bits);

	TRACE_DECODE(1, "rD = 0x%02x, A = 0x%02x", rR(D), rR(A));

	TAILCALL_NEXT();
}

static void vm_step_1_decode_type_r_rd_vra(vm_p vm)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode_op_rd_vra(vm, &opcode, &bits);

	TRACE_DECODE(1, "rD = 0x%02x, rA = 0x%02x", rR(D), rR(A));

	TAILCALL_NEXT();
}

static void vm_step_1_decode_type_r_rd_vra_rb_rc(vm_p vm)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode_op_rd_vra(vm, &opcode, &bits);
	_decode__rr(B, vm, &opcode, &bits);
	_decode__rr(C, vm, &opcode, &bits);

	TRACE_DECODE(1, "rD = 0x%02x, rA = 0x%02x, rB = 0x%02x, rC = 0x%02x",
		rR(D), rR(A), rR(B), rR(C));

	VV = opcode;

	TRACE_DECODE(1, "VV = 0x%08x", VV);

	TAILCALL_NEXT();
}

static void vm_step_1_decode_type_r_rd_vra_vrb(vm_p vm)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode_op_rd_vra_vrb(vm, &opcode, &bits);

	TRACE_DECODE(1, "rD = 0x%02x, rA = 0x%02x, rB = 0x%02x",
		rR(D), rR(A), rR(B));

	VV = opcode;

	TRACE_DECODE(1, "VV = 0x%08x", VV);

	TAILCALL_NEXT();
}

static void vm_step_1_decode_type_r_rd_vra_vrb_vrc(vm_p vm)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode_op_rd_vra_vrb(vm, &opcode, &bits);
	_decode__vr(C, vm, &opcode, &bits);

	TRACE_DECODE(1, "rD = 0x%02x, rA = 0x%02x, rB = 0x%02x, rC = 0x%02x",
		rR(D), rR(A), rR(B), rR(C));

	VV = opcode;

	TRACE_DECODE(1, "VV = 0x%08x", VV);

	TAILCALL_NEXT();
}

static void vm_step_1_decode_type_r_rd_vra_vrbsa(vm_p vm)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode_op_rd_vra_vrb(vm, &opcode, &bits);

	TRACE_DECODE(1, "rD = 0x%02x, rA = 0x%02x, rB = 0x%02x", rR(D), rR(A), rR(B));

	SA = rR(B) ? vR(B) : opcode;

	TRACE_DECODE(1, "SA = 0x%08x", SA);

	TAILCALL_NEXT();
}

static void vm_step_1_decode_type_r_vra(vm_p vm)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode_op_vra(vm, &opcode, &bits);

	TRACE_DECODE(1, "rA = 0x%02x", rR(A));
	
	TAILCALL_NEXT();
}

static void vm_step_1_decode_type_r_vra_vrb(vm_p vm)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode_op_vra_vrb(vm, &opcode, &bits);

	TRACE_DECODE(1, "rA = 0x%02x, rB = 0x%02x", rR(A), rR(B));

	VV = opcode;

	TRACE_DECODE(1, "VV = 0x%08x", VV);

	TAILCALL_NEXT();
}

/* **** */

static void vm_step_1_decode_type_u_nop(vm_p vm)
{
	uint8_t bits = 0;
	int32_t opcode = 0;

	_decode__op(vm, &opcode, &bits);

	VV = opcode;
		
	TAILCALL_NEXT();
}
