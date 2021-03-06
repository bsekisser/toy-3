#include <stdint.h>
#include <stdio.h>
#include <sys/syscall.h>

#include "bitfield.h"
#include "shift_roll.h"

#include "vm.h"

#include "vm_inst_esac_list.h"
#include "vm_inst_esac_enum.h"
#include "vm_inst_names.h"

#include "vm_trace.h"

#define DEBUG_CC_OP 0

static uint32_t _slif(uint32_t data, uint32_t ins, uint8_t bits) 
{
	return((_lsl(data, bits) & ~_BM(bits)) | (ins & _BM(bits)));
}

static uint32_t _pcrel(vm_p vm, int32_t pat)
{
	return((pat - (PC + 4)) >> 2);
}

/* **** */

static void cc_x32(vm_p vm, uint32_t data)
{
	*PXX(PC, 0, 4) = data;
}

/* **** */

#undef IR
#define IR op

#define cc_inst(_inst_) \
	INST_ENUM(_inst_)

static void cc_ia(vm_p vm, uint32_t op, uint32_t arg)
{
	uint8_t op_bits = IR_OP_BITS;
	uint32_t aarg = _slif(arg, 0, op_bits);
	
	if(0) TRACE("op_bits = 0x%02x, aarg = 0x%08x", op_bits, aarg);
	
	
	uint32_t ia = _slif(arg, op, op_bits);

	if(0) TRACE("(op = (0x%03x, %10s), arg = 0x%08x) --> 0x%08x",
			op, _inst_esac_name_list[op], arg, ia);

	cc_x32(vm, ia);

	op = ia;

	if(1) TRACE("0x%08x -- > (op = (0x%03x, %10s), arg = 0x%08x)",
			IR, IR_OP, _inst_esac_name_list[IR_OP], IR_ARG);
}

static void cc_ia_pcrel(vm_p vm, uint32_t op, int32_t pat)
{
	cc_ia(vm, op, _pcrel(vm, pat));
}

/* **** */

static void cc_op_r(vm_p vm, uint32_t op, uint32_t arg, uint8_t r0)
{
	uint32_t aarg = _slif(arg, r0, IR_REG_BITS);
	
	if(DEBUG_CC_OP) TRACE("op = 0x%08x, arg = 0x%08x, aarg = 0x%08x, r0 = 0x%02x",	op, arg, aarg, r0);

	cc_ia(vm, op, aarg);
}

static void cc_op_r_r(vm_p vm, uint32_t op, uint32_t arg, uint8_t r0, uint8_t r1)
{
	uint32_t aarg = _slif(arg, r1, IR_REG_BITS);

	if(DEBUG_CC_OP) TRACE("op = 0x%08x, arg = 0x%08x, aarg = 0x%08x, r0 = 0x%02x, r1 = 0x%02x",
		op, arg, aarg, r0, r1);

	cc_op_r(vm, op, aarg, r0);
}

static void cc_op_r_r_pcrel(vm_p vm, uint32_t op, uint32_t arg, uint8_t r0, uint8_t r1, int32_t pat)
{
	int32_t offset = _pcrel(vm, pat);
	uint32_t aarg = _slif(arg, offset, 16);

	if(DEBUG_CC_OP) TRACE("op = 0x%08x, arg = 0x%08x, aarg = 0x%08x, r0 = 0x%02x, r1 = 0x%02x, pat = 0x%08x, offset = 0x%08x",
		op, arg, aarg, r0, r1, pat, offset);

	cc_op_r_r(vm, op, aarg, r0, r1);
}

static void cc_op_r_r_i(vm_p vm, uint32_t op, uint32_t arg, uint8_t r0, uint8_t r1, int16_t i)
{
	uint32_t aarg = _slif(arg, i, 16);

	if(DEBUG_CC_OP) TRACE("op = 0x%08x, arg = 0x%08x, aarg = 0x%08x, r0 = 0x%02x, r1 = 0x%02x, i = 0x%08x",
		op, arg, aarg, r0, r1, i);

	cc_op_r_r(vm, op, aarg, r0, r1);
}

static void cc_op_r_r_r(vm_p vm, uint32_t op, uint32_t arg, uint8_t r0, uint8_t r1, uint8_t r2)
{
	uint32_t aarg = _slif(arg, r2, IR_REG_BITS);

	if(DEBUG_CC_OP) TRACE("op = 0x%08x, arg = 0x%08x, aarg = 0x%08x, r0 = 0x%02x, r1 = 0x%02x, r2 = 0x%02x",
		op, arg, aarg, r0, r1, r2);

	cc_op_r_r(vm, op, aarg, r0, r1);
}

/* **** */

#define cc_type_b(_op, _pat) \
	cc_ia_pcrel(vm, cc_inst(_op), _pat)

#define cc_type_i(_op, _d, _a, _i)  \
	cc_op_r_r_i(vm, cc_inst(_op), 0, _d, _a, _i)

#define cc_type_r(_op, _d, _a, _b) \
	cc_op_r_r_r(vm, cc_inst(_op), 0, _d, _a, _b)

#define cc_type_r_r_o(_op, _d, _a, _pat) \
	cc_op_r_r_pcrel(vm, cc_inst(_op), 0, _d, _a, _pat)

/* **** */

void pseudo_cc_init(vm_p vm)
{
	cc_type_r(xor, 1, 0, 0);
	cc_type_r(xor, 2, 0, 0);
	cc_type_i(addi, 3, 1, 15);
	
	PUSH(PC);
		cc_type_i(addi, 1, 1, 1);
		cc_type_i(subi, 2, 2, 1);
		cc_type_r_r_o(dbnz, 3, 3, POP());
//		cc_ia(nop, 0);
//		cc_op_rd_ra_pcea(dbeq, 0, 0, PC + 8);
		cc_type_b(bra, POP());
//		cc_op_rd_ra_pcea(dbnz, 0, 0, POP());
//		cc_ia(nop, 0);
//		cc_ia_rel(bra, pop());
//		cc_ia(syscall1, SYS_exit);
}
