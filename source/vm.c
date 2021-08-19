#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "bitfield.h"
#include "min_max.h"
#include "shift_roll.h"

#include "vm.h"

#include "vm_flags.h"

#include "vm_inst.h"
#include "vm_decode.h"

#include "vm_io.h"

#define TRACE(_f, args...) \
	printf("%s:(0x%08x, 0x%08x):0x%016llx:: " _f "\n", __FUNCTION__, PC, (PC - (int)&vm->rom), vm->cycle, ##args);
	
#undef INST_ESAC
#define INST_ESAC(_esac, _fn, _action) \
	static void vm_step_2_execute_##_esac(_PASS_VM, _PASS_INST) \
	{ \
		_action; \
	}

INST_ESAC_LIST

#undef INST_ESAC
#define INST_ESAC(_esac, _fn, _action) \
	vm_step_2_execute_##_esac,

static execute_fn_t vm_step_2_execute_fn[] = {
	INST_ESAC_LIST
};

#undef INST_ESAC

static vm_p vm_alloc(vm_h h2vm)
{
	*h2vm = calloc(1, sizeof(vm_t));
	
	return(*h2vm);
}

void vm_reset(_PASS_VM)
{
	vm->pc = (uint32_t**)&RFV(rPC);
	vm->sp = (uint32_t**)&RFV(rSP);
	
	pPC = (uint32_t*)&vm->rom[0];
	pSP = (uint32_t*)&vm->sdram[VM_SDRAM_ALLOC];

	vm->cycle = 0;

	vm->psr = 1 << PSR_BIT_US;
}

static int vm_step_0_fetch(_PASS_VM, _PASS_INST)
{
	IR = *pPC++;

	TRACE("IP = (0x%08x, 0x%08x), IR = 0x%08x", ((int)IP - (int)&vm->rom), (int)IP, IR);

	inst->decode_fn = vm_step_1_decode_fn[IR_OP];
	inst->execute_fn = vm_step_2_execute_fn[IR_OP];

	TRACE("IP = 0x%08x, IR = 0x%08x", (int)IP, IR);
	
	return(inst->decode_fn && inst->execute_fn);
}

static void _vm_step_3_io_memory_access_read(_PASS_VM, _PASS_INST)
{
	TRACE();

	if(!MA.io)
	{
		switch(MA.size)
		{
			case sizeof(uint8_t):
				RD = MA.is_signed ? *(int8_t*)EA : *(uint8_t*)EA;
				break;
			case sizeof(uint16_t):
				RD = MA.is_signed ? *(int16_t*)EA : *(uint16_t*)EA;
				break;
			default:
				RD = *(uint32_t*)EA;
				break;
		}
	}
	else
	{
		uint32_t data;
		
		data = io_read(vm, EA, MA.size);

		switch(MA.size)
		{
			case sizeof(uint8_t):
				RD = MA.is_signed ? (int8_t)data : (uint8_t)data;
				break;
			case sizeof(uint16_t):
				RD = MA.is_signed ? (int16_t)data : (uint16_t)data;
				break;
			default:
				RD = data;
		}
	}
}

static void _vm_step_3_io_memory_access_write(_PASS_VM, _PASS_INST)
{
	TRACE();

	if(!MA.io)
	{
		switch(MA.size)
		{
			case sizeof(uint8_t):
				*(uint8_t*)EA = RA;
				break;
			case sizeof(uint16_t):
				*(uint16_t*)EA = RA;
				break;
			case sizeof(uint32_t):
				*(uint32_t*)EA = RA;
				break;
		}
	}
	else
		io_write(vm, EA, RA, MA.size);
}

static void vm_step_3_io_memory_access(_PASS_VM, _PASS_INST)
{
	if(MA.rw & 0x01)
		_vm_step_3_io_memory_access_read(vm, inst);
	else if(MA.rw & 0x02)
		_vm_step_3_io_memory_access_write(vm, inst);
	else
		return;

	vm->cycle++;
}

static void vm_step_4_writeback_register(_PASS_VM, _PASS_INST)
{
	if(WBc) /* writeback -- c */
	{
		TRACE("WBc -- r%0u == 0x%08x", RCr, RC);
		RFV(RCr) = RC;
	}

	if(WBd) /* writeback -- d */
	{
		TRACE("WBd -- r%0u == 0x%08x", RDr, RD);
		RFV(RDr) = RD;
	}
}

int vm_step(_PASS_VM)
{
	IF_INST(vm_ixr_p) inst = &vm->inst;
	
	TRACE();
	
	if(vm_step_0_fetch(vm, inst))
	{
		inst->decode_fn(vm, inst);
		inst->execute_fn(vm, inst);
		vm_step_3_io_memory_access(vm, inst);
		vm_step_4_writeback_register(vm, inst);
		
		return(1);
	}

	vm->cycle++;

	return(0);
}

#define cc_x32(_x) \
	({ \
		TRACE("0x%08x", _x); \
		*CS(PC, 0, 1) = _x; \
	})

#define cc_inst(_inst_) \
	_inst_esac_##_inst_##_k

#define cc_ia(_inst_, _arg) \
	({ \
		uint32_t ia = (cc_inst(_inst_) & 0xff) | (((_arg) & 0x00ffffff) << 8); \
		TRACE("(op = (0x%08x, %s), arg = 0x%08x) --> 0x%08x", \
			cc_inst(_inst_), #_inst_, _arg, ia); \
		cc_x32(ia); \
	})

#define cc_ia_rel(_inst, _pat) \
	cc_ia(_inst, (signed)_pat - (PC + 4))

#define CS(_x, _pre, _post) \
	({ \
		uint32_t** tmp = (uint32_t**)&_x; \
		*tmp += _pre; \
		uint32_t* rval = *tmp; \
		*tmp += _post; \
		rval; \
	})

#define SS(_x) \
	*(uint32_t*)_x

#define pop() \
	SS(SP++)

#define push(_x) \
	SS(--SP) = _x	

int main(void)
{
	vm_p vm;

	vm_alloc(&vm);	
	vm_reset(vm);

	uint32_t* ip;

	push(PC);
		cc_ia(nop, 0);
		cc_ia_rel(bra, pop());

	vm_reset(vm);

	for(int i = 0; i < 15; i++)
		vm_step(vm);

	return(0);
}
