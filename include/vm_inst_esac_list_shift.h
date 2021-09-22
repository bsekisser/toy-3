#define SHIFT_ESAC_LIST \
	SHIFT_ESAC(asr) \
	SHIFT_ESAC(lsl) \
	SHIFT_ESAC(lsr) \
	SHIFT_ESAC(rol) \
	SHIFT_ESAC(ror) \
	SHIFT_ESAC(rlc) \
	SHIFT_ESAC(rrc)

/* **** */

#define _rlc(_in, _sa) \
	({ \
		int cf_out; \
		uint32_t out = _rol_c(_in, _sa, CF, &cf_out); \
		BMAS(PSR, PSR_BIT_CF, cf_out); \
		out; \
	})

#define _rrc(_in, _sa) \
	({ \
		int cf_out; \
		uint32_t out = _ror_c(_in, _sa, CF, &cf_out); \
		BMAS(PSR, PSR_BIT_CF, cf_out); \
		out; \
	})

/* **** */

#define SHIFT_ENUM(_enum) \
	_shift_enum_##_enum##_k

#undef SHIFT_ESAC
#define SHIFT_ESAC(_esac) \
	SHIFT_ENUM(_esac),

enum {
	SHIFT_ESAC_LIST
};

#undef SHIFT_ESAC

/* **** */

#undef SHIFT_ESAC
#define SHIFT_ESAC(_esac) \
	ESAC_ACTION(SHIFT_ENUM(_esac), vR(D) = _##_esac(vR(A), SA))

#define SHIFT(_esac) _shift_box(vm, SHIFT_ENUM(_esac))
static void _shift_box(vm_p vm, int esac)
{
	switch(esac)
	{
		SHIFT_ESAC_LIST
	}
}

#undef SHIFT_ESAC

/* **** */
