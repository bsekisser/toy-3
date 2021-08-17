#undef INST_ESAC
#define INST_ESAC(_esac, _fn, _action) \
	_esac##_k,

enum {
	INST_ESAC_LIST
};

#undef INST_ESAC
