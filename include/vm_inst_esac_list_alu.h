#define ALU_INST_ESAC_LIST \
	ALU_ESAC(add, RD += rbvv) \
	ALU_ESAC(adc, RD += (rbvv + CF)) \
	ALU_ESAC(and, RD &= rbvv) \
	ALU_ESAC(bic, RD &= ~rbvv) \
	ALU_ESAC(div, RD /= rbvv) \
	ALU_ESAC(eor, RD ^= rbvv) \
	ALU_ESAC(mod, RD %= rbvv) \
	ALU_ESAC(mov, (void)rbvv) \
	ALU_ESAC(mul, RD *= rbvv) \
	ALU_ESAC(orr, RD |= rbvv) \
	ALU_ESAC(sub, RD -= rbvv) \
	ALU_ESAC(sbc, RD -= (rbvv + CF))
