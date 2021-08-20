#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "bitfield.h"
#include "min_max.h"
#include "shift_roll.h"

#include "vm.h"

#include "vm_flags.h"

#include "vm_inst.h"
#include "vm_inst_names.h"

#include "vm_decode.h"

#include "vm_io.h"

#include "vm_trace.h"

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

/* **** */

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
	IR = *(IP = pPC++);
	
	uint32_t trip = IP - (uint32_t*)&vm->rom;

	TRACE("IP = (0x%08x, 0x%08x), IR = 0x%08x, OP = (0x%03x, %10s), ARG = 0x%08x",
		(int)IP, trip, IR,
		IR_OP, _inst_esac_name_list[IR_OP],
		IR_V24);

	inst->decode_fn = vm_step_1_decode_fn[IR_OP];
	inst->execute_fn = vm_step_2_execute_fn[IR_OP];

//	TRACE("IP = 0x%08x, IR = 0x%08x", (int)IP, IR);
	
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

	MA.rw = 0;
	vm->cycle++;
}

static void vm_step_4_writeback_register(_PASS_VM, _PASS_INST)
{
	if(WBc) /* writeback -- c */
	{
		WBc = 0;
		TRACE("WBc -- r%0u == 0x%08x", RCr, RC);
		RFV(RCr) = RC;
	}

	if(WBd) /* writeback -- d */
	{
		WBd = 0;
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
	}
	else
		return(-1);

	vm->cycle++;
	
	return(0);
}

int main(void)
{
	vm_p vm;

	vm_alloc(&vm);	
	vm_reset(vm);

	pseudo_cc_init(vm);

	vm_reset(vm);

	for(int i = 34; i > 0; i--)
		vm_step(vm);

	return(0);
}
