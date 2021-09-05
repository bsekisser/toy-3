#define BRA_CC(_x) \
	if(_x) PC = EA

#define CALL(_x) \
	LR = PC; PC = (_x)

#define DBRA_CC(_x) \
	({ \
		vR(D) = vR(A); vR(D)--; \
		BRA_CC(_x); \
	})

#define _INST_MA(_signed, _size) \
	({ \
		MA.is_signed = _signed; \
		MA.size = _size; \
	})

#define _INST_IO(_signed, _size) \
	({ \
		_INST_MA(_signed, _size); \
	})
	
#define INST_IO_Rs(_size) \
	_INST_IO(1, _size)

#define INST_IO_Ru(_size) \
	_INST_IO(0, _size)

#define INST_IO_W(_size) \
	_INST_IO(0, _size)

#define INST_MA_Rs(_size) \
	_INST_MA(1, _size)

#define INST_MA_Ru(_size) \
	_INST_MA(0, _size)

#define INST_MA_W(_size) \
	_INST_MA(0, _size)

#define LUI_ORI_NOP() \
	({ \
		if(rR(D)) \
			vR(D) = (vR(A) & _BM(16)) | IR_LUI; \
	})

#define MOV_CC(_x, _z) \
	if(_x) vR(D) = _z

#define PUSH(_x) \
	(*--pSP) = (uint32_t)(_x)

#define POP() \
	(*pSP++)

#define RTI() \
	RTS()

#define RTS() \
	PC = LR

/* **** */

#include "vm_inst_esac_list.h"
#include "vm_inst_esac_enum.h"

#include "vm_inst_esac_list_alu.h"
#include "vm_inst_esac_list_rrcc.h"
#include "vm_inst_esac_list_shift.h"
