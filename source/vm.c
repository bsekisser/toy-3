#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define IF_PIPELINE(_iff, _action) \
	if(_iff) ({ _action; })

#define _TAILCALL_NEXT() \
	({ \
		if(inst->step_next) { \
			vm_fn_t fn = *inst->step_next; \
			inst->step_next++; \
			return(fn(vm, inst)); \
		} \
	})

#define TAILCALL_NEXT() \
	IF_THREADED_DISPATCH(1, _TAILCALL_NEXT())

#define IF_THREADED_DISPATCH(_iff, _action) \
	if(!_iff) ({ _action; })

#define _TRACE_ALL 1
#define _TRACE_DECODE _TRACE_ALL
#define _TRACE_EXECUTE _TRACE_ALL
#define _TRACE_FETCH _TRACE_ALL
#define _TRACE_MEM _TRACE_ALL
#define _TRACE_WRITEBACK _TRACE_ALL

#include "bitfield.h"
#include "min_max.h"
#include "shift_roll.h"

#include "vm.h"
#include "vm_trace.h"

#include "vm_flags.h"

#include "vm_inst.h"
#include "vm_inst_names.h"

#include "vm_io.h"

#include "pseudo_cc_init.h"

#include "vm_step_4_writeback.h"
#include "vm_step_3_mem.h"
#include "vm_step_1_decode.h"
#include "vm_step_2_execute.h"

#undef INST_ESAC
#define INST_ESAC(_esac, _decode, _action, _mem, _wb) \
	[INST_ENUM(_esac)] = { \
		vm_step_1_decode_##_decode, \
		vm_step_2_execute_##_esac, \
		vm_step_3_##_mem, \
		vm_step_4_writeback_##_wb, \
	},

typedef struct vm_inst_fn_t* vm_inst_fn_p;
typedef struct vm_inst_fn_t {
	vm_fn_t		decode;
	vm_fn_t		execute;
	vm_fn_t		mem;
	vm_fn_t		wb;
}vm_inst_fn_t;

static vm_inst_fn_t vm_step_inst[256] = {
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
	vm->ppc = (uint32_t**)&vm->pc;
	vm->sp = (uint32_t**)&GPR(rSP);
	
	pPC = (uint32_t*)&vm->rom[0];
	pSP = (uint32_t*)&vm->sdram[VM_SDRAM_ALLOC];

	vm->cycle = 0;

	vm->psr = 1 << PSR_BIT_US;
}

static void vm_step_0_fetch(_PASS_VM, _PASS_INST)
{
	IR = *(IP = pPC++);
	
	uint32_t trip = IP - (uint32_t*)&vm->rom;

	if(_TRACE_FETCH) TRACE("IP = (0x%08x, 0x%08x), IR = 0x%08x, OP = (0x%03x, %10s), ARG = 0x%08x",
		(int)IP, trip, IR,
		IR_OP, _inst_esac_name_list[IR_OP],
		IR_ARG);

	vm_inst_fn_p vmifp = &vm_step_inst[IR_OP];

	inst->step_fn_list = (vm_fn_p)vmifp;
	inst->step_next = inst->step_fn_list;
	
	if(vmifp)
	{
		IF_THREADED_DISPATCH(1, TAILCALL_NEXT());
		IF_THREADED_DISPATCH(0, ({
			IF_PIPELINE(1, vmifp->decode(vm, inst));
			vmifp->execute(vm, inst);
			IF_PIPELINE(1, vmifp->mem(vm, inst));
			IF_PIPELINE(1, vmifp->wb(vm, inst));
		}));
	}

//	return((int)vmifp);
}

int vm_step(_PASS_VM)
{
	IF_INST(vm_ixr_p) inst = &vm->inst;
	
//	TRACE();
	
	vm_step_0_fetch(vm, inst);
	vm->cycle++;
	
	return(0);
}

int main(void)
{
	vm_p vm;

	vm_alloc(&vm);	
	vm_reset(vm);

	if(_TRACE_ALL)
	{
		printf("_inst_esac0_count = 0x%08x\n", _inst_esac0_count_k);
		printf("_inst_esac0_limit = 0x%08x\n", _inst_esac0_limit_k);
		printf("_inst_esac0_free  = 0x%08x\n", _inst_esac0_free_k);
		printf("\n");
		printf("_inst_esac1_start = 0x%08x\n", _inst_esac1_start_k);
		printf("_inst_esac1_end   = 0x%08x\n", _inst_esac1_end_k);
		printf("_inst_esac1_count = 0x%08x\n", _inst_esac1_count_k);
		printf("_inst_esac1_limit = 0x%08x\n", _inst_esac1_limit_k);
		printf("_inst_esac1_free  = 0x%08x\n", _inst_esac1_free_k);
		printf("\n");
		printf("_inst_esac_total  = 0x%08x\n", _inst_esac_total_k);

		printf("\n");

		vm_ixr_p inst = &vm->inst;

		IR = _inst_esac0_limit_k;
		printf("IR = 0x%08x -- IR_OP = 0x%08x, bits = 0x%02x\n", IR, IR_OP, IR_OP_BITS);
		
		IR = _inst_esac1_start_k;
		printf("IR = 0x%08x -- IR_OP = 0x%08x, bits = 0x%02x\n", IR, IR_OP, IR_OP_BITS);

		IR = _BV(IR_OP_BITS - 1);
		printf("IR = 0x%08x -- IR_OP = 0x%08x, bits = 0x%02x\n", IR, IR_OP, IR_OP_BITS);

		printf("\n");
	}

	pseudo_cc_init(vm);

	vm_reset(vm);

	int step_count = 0 ? MHz(100) :  32;
	for(int i = step_count; i > 0; i--)
		vm_step(vm);

	return(0);
}
