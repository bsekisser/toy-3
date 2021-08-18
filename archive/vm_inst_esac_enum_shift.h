#undef SHIFT_ESAC
#define SHIFT_ESAC(_esac, _action) \
	_alu_shift_esac_##_esac = _esac,

enum {
	SHIFT_ESAC_LIST
};

#undef SHIFT_ESAC
