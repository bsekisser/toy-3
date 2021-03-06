#define ALU_INST_ESAC_LIST \
	ALU_ESAC(add, vR(D) += rbvv) \
	ALU_ESAC(adc, vR(D) += (rbvv + CF)) \
	ALU_ESAC(sub, vR(D) -= rbvv) \
	ALU_ESAC(sbc, vR(D) -= (rbvv + CF)) \
	\
	ALU_ESAC(and, vR(D) &= rbvv) \
	ALU_ESAC(bic, vR(D) &= ~rbvv) \
	ALU_ESAC(orr, vR(D) |= rbvv) \
	ALU_ESAC(xor, vR(D) ^= rbvv) \
	\
	ALU_ESAC(bmas, vR(D) &= ~rbvv; vR(D) |= vR(C) & rbvv) \
	\
	ALU_ESAC(div, vR(D) /= rbvv) \
	ALU_ESAC(mod, vR(D) %= rbvv) \
	ALU_ESAC(mul, vR(D) *= rbvv)

#define ALU_Q_INST_ESAC_LIST \
	ALU_ESAC(mult, Q = vR(A) * vR(B)) \
	ALU_ESAC(multu, Q = (unsigned)vR(A) * (unsigned)vR(B)) \
	ALU_ESAC(divu, Q = 0; LO = (uint32_t)vR(A) / (uint32_t)vR(B)) \
	ALU_ESAC(divmod, HI = vR(A) % vR(B); LO = vR(A) / vR(B)) \
	\
	ALU_ESAC(madd, Q += (vR(A) * vR(B))) \
	ALU_ESAC(maddu, Q += ((unsigned)vR(A) * (unsigned)vR(B))) \
	ALU_ESAC(msub, Q -= (vR(A) * vR(B))) \
	ALU_ESAC(msubu, Q -= ((unsigned)vR(A) * (unsigned)vR(B)))
	
#undef ALU_ESAC
#define ALU_ESAC(_esac, _action) \
	ESAC_ACTION(INST_ENUM(_esac), _action)

static void alu(vm_p vm, uint8_t operation, uint32_t rbvv, int set_flags)
{
	vR(D) = vR(A);
	
	switch(operation)
	{
		ALU_INST_ESAC_LIST
	}
	
	if(set_flags)
	{
		switch(operation)
		{
			case	INST_ENUM(add):
			case	INST_ENUM(adc):
				alu_flag_fn_add(vm, vR(D), vR(A), rbvv);
				break;
			case	INST_ENUM(sub):
			case	INST_ENUM(sbc):
				alu_flag_fn_sub(vm, vR(D), vR(A), rbvv);
				break;
			default:
				alu_flag_fn_nz(vm, vR(D));
		}
	}
}

static void alu_q(vm_p vm, uint8_t operation)
{
	switch(operation)
	{
		ALU_Q_INST_ESAC_LIST
	}
}

#undef ALU_ESAC

#define ALU(_esac) \
	alu(vm, INST_ENUM(_esac), vR(B), BEXT(IR, 31))

#define ALU_I(_esac) \
	alu(vm, INST_ENUM(_esac), VV, 0)

#define ALU_Iu(_esac) \
	alu(vm, INST_ENUM(_esac), VV & 0xffff, 0)


#define ALU_Q(_esac) \
	alu_q(vm, INST_ENUM(_esac))

#define ALU_RBVV(_esac, _rbvv, _set_flags) \
	alu(vm, INST_ENUM(_esac), _rbvv, _set_flags)
