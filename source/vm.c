#include <stdint.h>

#include "bitfield.h"
#include "min_max.h"
#include "shift_roll.h"

#if 1
	#define IF_INST(_x) _x
	#define IF_VM(_x) _x

	#define _PASS_INST vm_ixr_p inst
	#define _PASS_VM vm_p vm
#elif 0
	register vm_ixr_p inst asm("r5");
	register vm_p vm asm("r4");

	#define IF_INST(_x)
	#define IF_VM(_x)

	#define _PASS_INST void* _pass_inst
	#define _PASS_VM void* _pass_vm
#else
	vm_ixr_p inst;
	vm_p vm;
	
	#define IF_INST(_x)
	#define IF_VM(_x)

	#define _PASS_INST void* _pass_inst
	#define _PASS_VM void* _pass_vm
#endif

#include "vm.h"

#include "vm_flags.h"

#include "vm_inst.h"
#include "vm_decode.h"

#include "vm_io.h"

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

typedef void (*execute_fn_t)(_PASS_VM, _PASS_INST);

static execute_fn_t vm_step_2_execute_fn[] = {
	INST_ESAC_LIST
};

#undef INST_ESAC

static void vm_step_0_fetch(_PASS_VM, _PASS_INST)
{
	IP = PC++;
	IR = *(uint32_t*)IP;
}

static void _vm_step_3_io_memory_access_read(_PASS_VM, _PASS_INST)
{
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
		RFV(RCr) = RC;

	if(WBd) /* writeback -- d */
		RFV(RDr) = RD;
}

int vm_step(_PASS_VM)
{
	IF_INST(vm_ixr_p) inst = &vm->inst;
	
	vm_step_0_fetch(vm, inst);
	vm_step_1_decode_fn[IR_OP](vm, inst);
	vm_step_2_execute_fn[IR_OP](vm, inst);
	vm_step_3_io_memory_access(vm, inst);
	vm_step_4_writeback_register(vm, inst);

	vm->cycle++;

	return(0);
}

int main(void)
{
	vm_t vvm;
	IF_VM(vm_p) vm = &vvm;

	for(;;)
		vm_step(vm);

	return(0);
}
