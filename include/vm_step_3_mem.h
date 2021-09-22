#define VM_STEP_3_IO_LD(_esac, _type) \
	static void vm_step_3_io_ld##_esac(vm_p vm) \
	{ \
		vR(D) = (_type)io_read(vm, EA, sizeof(_type)); \
	}

VM_STEP_3_IO_LD(8s, int8_t)
VM_STEP_3_IO_LD(8u, uint8_t)
VM_STEP_3_IO_LD(16s, int16_t)
VM_STEP_3_IO_LD(16u, uint16_t)
VM_STEP_3_IO_LD(32, uint32_t)

#undef VM_STEP_3_IO_LD

/* **** */

#define VM_STEP_3_IO_ST(_esac, _type) \
	static void vm_step_3_io_st##_esac(vm_p vm) \
	{ \
		io_write(vm, EA, vR(D), sizeof(_type)); \
	}

VM_STEP_3_IO_ST(8, uint8_t)
VM_STEP_3_IO_ST(16, uint16_t)
VM_STEP_3_IO_ST(32, uint32_t)

#undef VM_STEP_3_IO_ST

/* **** */

#define VM_STEP_3_MA_LD(_esac, _type) \
	static void vm_step_3_ma_ld##_esac(vm_p vm) \
	{ \
		vR(D) = *(_type *)EA; \
	}

VM_STEP_3_MA_LD(8s, int8_t)
VM_STEP_3_MA_LD(8u, uint8_t)
VM_STEP_3_MA_LD(16s, int16_t)
VM_STEP_3_MA_LD(16u, uint16_t)
VM_STEP_3_MA_LD(32, uint32_t)

#undef VM_STEP_3_MA_LD

/* **** */

#define VM_STEP_3_MA_ST(_esac, _type) \
	static void vm_step_3_ma_st##_esac(vm_p vm) \
	{ \
		*(_type *)EA = vR(A); \
	}

VM_STEP_3_MA_ST(8, uint8_t)
VM_STEP_3_MA_ST(16, uint16_t)
VM_STEP_3_MA_ST(32, uint32_t)

#undef VM_STEP_3_MA_ST

/* **** */

static void vm_step_3_nop(vm_p vm)
{
	TAILCALL_NEXT();
}
