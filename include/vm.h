#define Kb(_x)		((_x) * 1024UL)
#define Mb(_x)		Kb(Kb(_x))
#define Gb(_x)		Mb(Kb(_x))

#define KHz(_x)		((_x) * 1000UL)
#define MHz(_x)		KHz(KHz(_x))

enum {
	rBP = 0x0c,
	rLR = 0x0d,
	rSP = 0x0e,
	rPC = 0x0f,
};

/*
	0 -- 1 -- IF -- fetch -- writeback
	1 -- 2 -- ID -- decode
	2 -- 3 -- EX -- execute -- (fetch -- writeback)
	3 -- 4 -- MA -- memory access
	5 -- 5 -- WB -- writeback

	0 -- IF0 -- ID0 -- EX0 -- IF1/WB0 -- ID1 -- EX1 -- IF2/WB1
	1 -- ID0 -- EX0 -- IF1/WB0 -- ID1 -- EX1 -- IF2/WB1 -- ID2
	2 -- EX0 -- IF1/WB0 -- ID1 -- EX1 -- IF2/WB1 -- ID2 -- EX2
	3 -- IF1/WB0 -- ID1 -- EX1 -- IF2/WB1 -- ID2 -- EX2 -- IF3/WB2
	4 -- ID1 -- EX1 -- IF2/WB1 -- ID2 -- EX2 -- IF3/WB2 -- ID3

	ea_xx_xx --	eeee eeee | eeee eeee | eeee eeee | oooo oooo
	rd_ra_ea -- eeee eeee | eeee eeee | a1a1 dddd | oooo oooo
	rd_ra_rb -- vvvv vvvv | cccc b2b2 | a1a1 dddd | oooo oooo
	rd_ra_vv -- vvvv vvvv | vvvv vvvv | a1a1 dddd | oooo oooo
	rd_ra_zz -- xxxx xxxx | xxxx xxxx | a1a1 dddd | oooo oooo
	rd_vv_vv -- vvvv vvvv | vvvv vvvv | vvvv dddd | oooo oooo
	rd_zz_zz -- xxxx xxxx | xxxx xxxx | xxxx dddd | oooo oooo
	ra_zz_zz -- xxxx xxxx | xxxx xxxx | xxxx a1a1 | oooo oooo
	ra_rb_ea -- eeee eeee | eeee eeee | b2b2 a1a1 | oooo oooo

	rd_ra_rb_rc -- vvvv vvvv | c3c3 b2b2 | a1a1 dddd | oooo oooo

	a1 -- src reg
	b2 -- src2 reg
	c3 -- src3 reg
	d -- dest reg
	e -- effective address offset
	o -- opcode
	v -- signed / unsigned immediate value
	x,z -- dont care

	cccc -- zf of nf cf

*/

#define IR0		(IR)
#define IR1		(IR >> 8)
#define IR1h	(IR >> 12)
#define IR2		(IR >> 16)
#define IR2h	(IR >> 20)
#define IR3		(IR >> 24)

#define IR_OP	(IR0 & 0xff)

#define IR_CC	(IR2h & 0x0f)
#define IR_RD	(IR1 & 0x0f)
#define IR_RA	(IR1h & 0x0f)
#define IR_RB	(IR2 & 0x0f)
#define IR_RC	(IR_CC)

#define IR_V8	(IR3 & 0x000000ff)
#define IR_V16	(IR2 & 0x0000ffff)
#define IR_V16s	(((signed)IR >> 16) & 0x0000ffff)
#define IR_V20	(IR2h & 0x000fffff)
#define IR_V24	(IR1 & 0x00ffffff)
#define IR_V24s	(((signed)IR >> 8) & 0x00ffffff)

#define R(_x) inst->rr[_x]
#define RFV(_x) vm->rf[_x]
#define V(_x) inst->rv[_x]

#define WB vm->flags.xx.wb

enum {
	PSR_BIT_CF,
	PSR_BIT_NF,
	PSR_BIT_OVF,
	PSR_BIT_ZF,
	PSR_BIT_IE = 16,		/* interrupt enable */
	PSR_BIT_IX,				/* instruction exception */
	PSR_BIT_ST,				/* instruction step */
	PSR_BIT_US,				/* user/supervisor */
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

#define EA inst->ea
#define IP inst->ip
#define IR inst->ir
#define VV inst->vv

#define BP RFV(rBP)
#define LR RFV(rLR)
#define PC RFV(rPC)
#define PSR vm->flags.psr
#define SP RFV(rSP)
//#define TEA vm->tea

#define RD V(0)
#define RA V(1)
#define RB V(2)
#define RC V(3)

#define RDr R(0)
#define RAr R(1)
#define RBr R(2)
#define RCr R(3)

typedef struct trap_table_t {
	union {
		struct {
			uint32_t	cold_reset;			//	0004	0000
			uint32_t	warm_reset;			//	0004	0008
		};
		
		uint32_t	interrupt[16];		//	0064	0072
	};
}trap_table_t;

typedef struct vm_ixr_t* vm_ixr_p;
typedef struct vm_ixr_t {
//		vm_p				vm;

		uint8_t				rr[4];
		uint32_t			rv[4];

		uint32_t			ip;
		uint32_t			ir;

		uint8_t				cc;
		union {
			uint32_t		ea;
			uint32_t		vv;
		};
}vm_ixr_t;

#define VM_NVRAM_ALLOC		256
#define VM_ROM_ALLOC		Kb(32)
#define VM_SDRAM_ALLOC		Mb(1)

#define VM_NVRAM_MASK		(VM_NVRAM_ALLOC - 1)
#define VM_ROM_MASK			(VM_ROM_ALLOC - 1)
#define VM_SDRAM_MASK		(VM_SDRAM_ALLOC - 1)

#define VM_NVRAM_SLOT		0x1f

#define VM_DEVICE_SLOT(_pat)	(((_pat) >> 8) & 0x1f)
#define VM_DEVICE_OFFSET(_pat)	((_pat) & 0xff)

typedef struct vm_t* vm_p;
typedef struct vm_t {
	vm_ixr_t	inst;

	uint32_t				rf[16];
//	uint32_t				tea;

	struct {
		uint32_t			psr;

		struct {
//			uint8_t			cw:1; /* cold / warm boot */
			uint8_t			ie:1; /* interrupt enable */
//			uint8_t			ix:1; /* instruction exception */
//			uint8_t			ii:1; /* set on interrupt, rfi cleared */
			uint8_t			st:1; /* step instruction */
			uint8_t			us:1; /* user / supervisor */
		}cr;
		
		struct {
//			uint8_t			ps:1; /* pipeline stall */
			uint8_t			wb:1; /* writeback */
		}xx;
	}flags;
	
	uint8_t					sdram[VM_SDRAM_ALLOC];
	uint8_t					nvram[VM_NVRAM_ALLOC];
	uint8_t					rom[VM_ROM_ALLOC];
}vm_t;

int vm_interrupt(vm_p vm);
int vm_reset(vm_p vm);
int vm_step(_PASS_VM);
