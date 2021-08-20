#include <stdint.h>
#include <stdio.h>
#include <sys/syscall.h>

#include "bitfield.h"
//#include "shift_roll.h".

#include "vm.h"

#include "vm_inst_esac_list.h"
#include "vm_inst_esac_enum.h"

#include "vm_trace.h"

//		TRACE("0x%08x", _x);
#define cc_x32(_x) \
	({ \
		*pPC++ = _x; \
	})

#define cc_inst(_inst_) \
	_inst_esac_##_inst_##_k

#define cc_ia(_inst_, _arg) \
	({ \
		uint32_t ia = (cc_inst(_inst_) & 0xff) | (((_arg) & 0x00ffffff) << 8); \
		TRACE("(op = (0x%03x, %10s), arg = 0x%08x) --> 0x%08x", \
			cc_inst(_inst_), #_inst_, _arg, ia); \
		cc_x32(ia); \
	})

#define _pcrel(_pat) \
	((signed)_pat - (PC + 4))

#define cc_ia_rel(_inst, _pat) \
	cc_ia(_inst, _pcrel(_pat))


#define pop() \
	*pSP++

#define push(_x) \
	*--pSP = _x	

#define cc_op_v4_v20(_op, _v4, _v20) \
	({ \
		uint32_t _v4v20 = _LSL(_v20, 4) | (_v4 & 0x0f); \
		cc_ia(_op, _v4v20); \
	})

#define cc_op_v4_v4_v16(_op, _v4a, _v4b, _v16) \
	({ \
		uint32_t _v4v16 = _LSL(_v16, 4) | (_v4b & 0x0f); \
		cc_op_v4_v20(_op, _v4a, _v4v16); \
	})

#define cc_op_ra_rb_pcea(_op, _ra, _rb, _vv) \
		cc_op_v4_v4_v16(_op, _ra, _rb, _pcrel(_vv))
		
#define cc_op_rd_ra_pcea(_op, _rd, _ra, _vv) \
		cc_op_v4_v4_v16(_op, _rd, _ra, _pcrel(_vv))

#define cc_op_rd_vv_vv(_op, _rd, _vv) \
		cc_op_v4_v20(_op, _rd, _vv)

#define cc_op_rd_ra_vv(_op, _rd, _ra, _vv) \
		cc_op_v4_v4_v16(_op, _rd, _ra, _vv)

void pseudo_cc_init(_PASS_VM)
{
	uint32_t* ip;

	cc_op_rd_vv_vv(liu, 0, 15);
	push(PC);
//		cc_op_rd_ra_vv(subi, 0, 0, 1);
//		cc_ia(nop, 0);
		cc_op_rd_ra_pcea(dbeq, 0, 0, PC + 8);
		cc_ia_rel(bra, pop());
//		cc_op_rd_ra_pcea(dbnz, 0, 0, pop());
		cc_ia(nop, 0);
//		cc_ia_rel(bra, pop());
		cc_ia(syscall1, SYS_exit);
}
