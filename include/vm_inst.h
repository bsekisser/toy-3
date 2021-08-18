#define BRA_CC(_x) \
	if(_x) PC = EA

#define CALL(_x) \
	LR = PC; PC = (_x)

#define INST_MA(_io, _signed, _size) \
	({ \
		MA.io = _io; \
		MA.is_signed = _signed; \
		MA.size = _size; \
	})

#define MOV_CC(_x, _z) \
	if(_x) RD = _z

#define PUSH(_x) \
	(*(uint32_t*)--SP) = (uint32_t)(_x)

#define POP() \
	(*(uint32_t*)SP++)

#define RTI() \
	RTS()

#define RTS() \
	PC = LR

/* **** */

#include "vm_inst_esac_list.h"
#include "vm_inst_esac_enum.h"

#include "vm_inst_esac_list_alu.h"
#include "vm_inst_esac_list_shift.h"

/* **** */

#undef ALU_ESAC
#define ALU_ESAC(_esac, _action) \
	case	_inst_esac_##_esac##_k: \
		_action; \
		break;

static void alu(_PASS_VM, _PASS_INST, uint8_t operation, uint32_t rbvv, int set_flags)
{
	RD = RA;
	
	switch(operation)
	{
		ALU_INST_ESAC_LIST
	}
	
	if(set_flags)
	{
		switch(operation)
		{
			case	_inst_esac_add_k:
			case	_inst_esac_adc_k:
				alu_flag_fn_add(vm, RD, RA, rbvv);
				break;
			case	_inst_esac_and_k:
			case	_inst_esac_bic_k:
			case	_inst_esac_eor_k:
			case	_inst_esac_orr_k:
				alu_flag_fn_nz(vm, RD);
				break;
			case	_inst_esac_sub_k:
			case	_inst_esac_sbc_k:
				alu_flag_fn_sub(vm, RD, RA, rbvv);
				break;
		}
	}
}

#undef ALU_ESAC

#define ALU(_esac, _rbvv, _set_flags) \
	alu(vm, inst, _inst_esac_##_esac##_k, _rbvv, _set_flags)

/* **** */

#undef SHIFT_ESAC
#define SHIFT_ESAC(_esac, _action) \
	static void alu_shift_##_esac(_PASS_VM, _PASS_INST, uint8_t count) \
	{ \
		_action; \
	}

SHIFT_ESAC_LIST

#undef SHIFT_ESAC

/* **** */

#define ALU_SHIFT(_esac, _count) \
	alu_shift_##_esac(vm, inst, _count)
