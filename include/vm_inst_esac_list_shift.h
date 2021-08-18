#define SHIFT_ESAC_LIST \
	SHIFT_ESAC(asr, RD = _ASR(RA, count)) \
	SHIFT_ESAC(lsl, RD = _LSL(RA, count)) \
	SHIFT_ESAC(lsr, RD = _LSR(RA, count)) \
	SHIFT_ESAC(rlc, RD = _rol_c(RA, count, CF)) \
	SHIFT_ESAC(rol, RD = _rol(RA, count)) \
	SHIFT_ESAC(ror, RD = _ror(RA, count)) \
	SHIFT_ESAC(rrc, RD = _ror_c(RA, count, CF))
