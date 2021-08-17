#include <stdint.h>

#include "bitfield.h"
#include "min_max.h"
#include "shift_roll.h"

#include "vm.h"

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

#include "vm_inst.h"
#include "vm_inst_enum.h"

#include "vm_decode.h"

uint32_t io_read(_PASS_VM, uint32_t pat, uint8_t size)
{
	uint32_t dout = 0;

	for(int i = 0; i < size; i++)
	{
		uint32_t slot = VM_DEVICE_SLOT(pat);
		uint16_t offset = VM_DEVICE_OFFSET(pat);

		dout <<= 8;

		switch(slot)
		{
			case	VM_NVRAM_SLOT:
				dout |= vm->nvram[offset & VM_NVRAM_MASK];
				break;
		}

		pat++;
	}
	return(dout);
}

void io_write(_PASS_VM, uint32_t pat, uint32_t data, uint8_t size)
{
	for(int i = 0; i < size; i++)
	{
		uint32_t slot = VM_DEVICE_SLOT(pat);
		uint16_t offset = VM_DEVICE_OFFSET(pat);
		
		uint8_t dout = (data >> (i << 3)) & 0xff;
		
		switch(slot)
		{
			case	VM_NVRAM_SLOT:
				vm->nvram[offset & VM_NVRAM_MASK] = dout;
				break;
		}
		
		pat++;
	}
}

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

int vm_step(_PASS_VM)
{
	IF_INST(vm_ixr_p) inst = &vm->inst;
	
	vm_step_0_fetch(vm, inst);
	vm_step_1_decode_fn[IR_OP](vm, inst);
	vm_step_2_execute_fn[IR_OP](vm, inst);
	
	if(WB) /* writeback */
		RFV(RDr) = RD;

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
