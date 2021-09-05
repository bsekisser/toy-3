#define SHIFT_ESAC_LIST \
	SHIFT_ESAC(asr, vR(D) = _asr(vR(A), count)) \
	SHIFT_ESAC(lsl, vR(D) = _lsl(vR(A), count)) \
	SHIFT_ESAC(lsr, vR(D) = _lsr(vR(A), count)) \
	SHIFT_ESAC(rlc, int cout; vR(D) = _rol_c(vR(A), count, CF, &cout); BMAS(PSR, PSR_BIT_CF, cout)) \
	SHIFT_ESAC(rol, vR(D) = _rol(vR(A), count)) \
	SHIFT_ESAC(ror, vR(D) = _ror(vR(A), count)) \
	SHIFT_ESAC(rrc, int cout; vR(D) = _ror_c(vR(A), count, CF, &cout); BMAS(PSR, PSR_BIT_CF, cout))

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
