#undef INST_ESAC
#define INST_ESAC(_esac, _type, _decode, _action, _mem, _wb) \
	INST_ENUM(_esac),

enum {
	INST_ESAC_0_LIST
	_inst_esac0_count_k,
	_inst_esac0_limit_k = _BM(IR_OP_BITS - 1),
	INST_ESAC_1_LIST
	_inst_esac1_end_k,
	_inst_esac1_limit_k = _BM(IR_OP_BITS),
	/* **** */
	_inst_esac0_free_k = _inst_esac0_limit_k - _inst_esac0_count_k,
	/* **** */
	_inst_esac1_start_k = _BV(IR_OP_BITS - 1),
	_inst_esac1_count_k = _inst_esac1_end_k - _inst_esac1_start_k,
	_inst_esac1_free_k = _inst_esac1_limit_k - _inst_esac1_end_k,
	/* **** */
	_inst_esac_total_k = _inst_esac0_count_k + _inst_esac1_count_k,
};

#undef INST_ESAC
