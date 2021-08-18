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

static void vm_step_3_io_memory_access(_PASS_VM, _PASS_INST)
{
	uint32_t dout = RA;

	if(MA) /* writeback -- ma */
	{
		if(MA.is_signed)
		{
			switch(MA.size)
			{
				case	sizeof(int8_t):
					dout = (int8_t)RA;
					break;
				case	sizeof(int16_t):
					dout = (int16_t)RA;
					break;
			}
		}

		if(!MA.io)
		{
			switch(MA.size)
			{
				case sizeof(uint8_t):
					*(uint8_t*)EA = dout;
					break;
				case sizeof(uint16_t):
					*(uint16_t*)EA = dout;
					break;
				case sizeof(uint32_t):
					*(uint32_t*)EA = dout;
					break;
			}
		} else
			io_write(vm, EA, dout, MA.size);

		vm->cycle++;
	}
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
