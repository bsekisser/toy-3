#define Kb(_x)		((_x) * 1024UL)
#define Mb(_x)		Kb(Kb(_x))
#define Gb(_x)		Mb(Kb(_x))

#define KHz(_x)		((_x) * 1000UL)
#define MHz(_x)		KHz(KHz(_x))

#define REG_BITS 5
#define IR_REG_BITS ((IR & 0x80) ? IR_REG1_BITS : IR_REG0_BITS)
#define IR_REG0_BITS 4
#define IR_REG1_BITS REG_BITS

enum {
		rGP = 0x1c,
		rSP = 0x1d,
#define SP GPR(rSP)
		rFP = 0x1e,
		rRA = 0x1f,
		
		/* aka */
		rBP = rFP,
#define BP GPR(rBP)
		rLR = rRA,
#define LR GPR(rLR)
};

enum {
	rSPR_HI,
	rSPR_LO,
	rSPR_PC = 0x1f,
	/* **** */
	rSPR_Q = rSPR_HI,
};

enum {
	_rRD,
	_rRA,
	_rRB,
	_rRC
};

/*
	0 -- 1 -- IF -- fetch -- writeback
	1 -- 2 -- ID -- decode
	2 -- 3 -- EX -- execute
	3 -- 4 -- MA -- memory access
	4 -- 5 -- WB -- writeback

	
	type_pcea			--  eeee eeee | eeee eeee | eeee eeee | 0ooo oooo
	type_rd_ra_i		--	iiii iiii | iiii iiii | aaaa dddd | 0ooo oooo
	type_rd_ra_u		--	uuuu uuuu | uuuu uuuu | aaaa dddd | 0ooo oooo
	type_rd_ra_rb		--	0000 0000 | 0bbb bbaa | aaa ddddd | 1ooo oooo
	type_rd_ra_rb_rc	--	0000 cccc | cbbb bbaa | aaa ddddd | 1ooo oooo


	a -- src reg
	b -- src2 reg
	c -- src3 reg
	d -- dest reg
	e -- effective address offset
	f -- set flags
	i -- signed value
	u -- unsigned value
	o -- opcode

*/

enum {
	/* condition code flags */
		PSR_BIT_CF,
		PSR_BIT_NF,
		PSR_BIT_OVF,
		PSR_BIT_ZF,

	/* user mode flags */
		PSR_BIT_US = 8,				/* user/supervisor */

	/* supervisor mode flags */
		PSR_BIT_IE = 16,		/* interrupt enable */
		PSR_BIT_IX,				/* instruction exception */
		PSR_BIT_ST,				/* instruction step */
};

#define PSR_CF		(1 << PSR_BIT_CF)
#define PSR_NF		(1 << PSR_BIT_NF)
#define PSR_OVF		(1 << PSR_BIT_OVF)
#define PSR_ZF		(1 << PSR_BIT_ZF)

#define PSR_NZ		(PSR_NF | PSR_ZF)
#define PSR_CNVZ	(PSR_NZ | PSR_CF | PSR_OVF)

#define CF			(!!(PSR & PSR_CF))
#define NF			(!!(PSR & PSR_NF))
#define OVF			(!!(PSR & PSR_OVF))
#define ZF			(!!(PSR & PSR_ZF))

#define IR_OP		(IR & 0xff)
#define IR_OP_BITS	8
#define IR_ARG		_ASR(IR, IR_OP_BITS)
#define IR_LUI		(IR & ~_BM(16))

typedef struct trap_table_t {
	union {
		struct {
			uint32_t	cold_reset;
			uint32_t	warm_reset;
			uint32_t	vertical_blank;
			uint32_t	horizontal_blank;
		};
		
		uint32_t	interrupt[16];		//	0064	0072
	};
}trap_table_t;

typedef struct vm_t** vm_h;
typedef struct vm_t* vm_p;

typedef void (**vm_fn_p)(vm_p vm);
typedef void (*vm_fn_t)(vm_p vm);

typedef struct vm_ixr_t* vm_ixr_p;
typedef struct vm_ixr_t {
//		vm_p				vm;

		uint32_t			v[4];
#define vR(_x)				IXR->v[_rR##_x]
#define SA					vR(C)

		uint32_t*			ip;
#define IP					IXR->ip
		uint32_t			ir;
#define IR					IXR->ir

		vm_fn_p				step_next;
		vm_fn_p				step_fn_list;

		union {
			uint32_t		ea;
#define EA					IXR->ea

			uint32_t		vv;
#define VV					IXR->vv
		};

		uint8_t				r[4];
#define rR(_x)				IXR->r[_rR##_x]
}vm_ixr_t;

#define VM_NVRAM_ALLOC		256
#define VM_ROM_ALLOC		Kb(32)
#define VM_SDRAM_ALLOC		Mb(1)

#define VM_NVRAM_BITS		VM_NVRAM_ALLOC >> 3
#define VM_ROM_BITS			VM_ROM_ALLOC >> 3
#define VM_SDRAM_BITS		VM_SDRAM_ALLOC >> 3

#define VM_NVRAM_MASK		(VM_NVRAM_ALLOC - 1)
#define VM_ROM_MASK			(VM_ROM_ALLOC - 1)
#define VM_SDRAM_MASK		(VM_SDRAM_ALLOC - 1)

#define VM_NVRAM_SLOT		0x1f

#define VM_DEVICE_SLOT(_pat)	(((_pat) >> 8) & 0x1f)
#define VM_DEVICE_OFFSET(_pat)	((_pat) & 0xff)

typedef struct vm_t {
	vm_ixr_t				ixr;
#define IXR					(&vm->ixr)

	uint64_t				cycle;

	union {
		uint64_t			spr_q[_BV(REG_BITS - 1)];
#define SPR_Q(_x)			vm->spr_q[_x & _BM(REG_BITS - 1)]
		uint32_t			spr[_BV(REG_BITS)];
#define SPR(_x)				vm->spr[_x & _BM(REG_BITS)]

#define rSPR(_x) SPR(rSPR_##_x)

#define HI SPR(rSPR(HI))
#define LO SPR(rSPR(LO))
#define Q SPR(rSPR(Q))
	};

	uint32_t				gpr[_BV(REG_BITS)];
#define GPR(_x)				vm->gpr[_x & _BM(REG_BITS)]

	uint32_t				psr;
#define PSR vm->psr
	
	uint8_t					sdram[VM_SDRAM_ALLOC];
	uint8_t					nvram[VM_NVRAM_ALLOC];
	uint8_t					rom[VM_ROM_ALLOC];
}vm_t;

#define PC					rSPR(PC)
#define pPC					(uint32_t*)PC

#define PXX(_xx, _pre, _post) \
	({ \
		_xx += _pre; \
		uint32_t* ppc = (uint32_t*)_xx; \
		_xx += _post; \
		ppc; \
	})

#define POP() \
	*PXX(SP, 0, 4)

#define PUSH(_x) \
	*PXX(SP, -4, 0) = (uint32_t)_x;

void vm_interrupt(vm_p vm);
void vm_reset(vm_p vm);
int vm_step(vm_p vm);
