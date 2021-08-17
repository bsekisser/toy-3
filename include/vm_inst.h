#if 1
	#define BRA_CC(_x) \
		if(_x) PC = EA
#else
	static void _bra_cc(_PASS_VM, _PASS_INST, int test)
	{
		if(test)
			PC = EA;
	}
	#define BRA_CC(_x) _bra_cc(vm, inst, _x)
#endif

#if 1
	#define CALL(_x) \
		LR = PC; PC = (_x)
#else
	static void _call(_PASS_VM, _PASS_INST, uint32_t ea)
	{
		LR = PC;
		PC = EA;
	}
	#define CALL(_x) _call(vm, inst, _x)
#endif

#define MOV_CC(_x, _z) \
	if(_x) RD = _z

#define PUSH(_x) \
	(*(uint32_t*)--SP) = (uint32_t)(_x)

#define POP() \
	(*(uint32_t*)SP++)

#define RTI() \
	RTS()

#define RTS() \
	PC = LR

enum {
	__add,
	__adc,
	__and,
	__bic,
	__div,
	__eor,
	__mod,
	__mov,
	__mul,
	__orr,
	__sub,
	__sbc,
};

static void alu(_PASS_VM, _PASS_INST, uint8_t operation, uint32_t rbvv)
{
	RD = RA;
	
	switch(operation)
	{
		case	__add:
			RD += rbvv;
			break;
		case	__adc:
			RD += (rbvv + CF);
			break;
		case	__and:
			RD &= rbvv;
			break;
		case	__bic:
			RD &= ~rbvv;
			break;
		case	__div:
			RD /= rbvv;
			break;
		case	__eor:
			RD ^= rbvv;
			break;
		case	__mod:
			RD %= rbvv;
			break;
		case	__mov:
			break;
		case	__mul:
			RD *= rbvv;
			break;
		case	__orr:
			RD |= rbvv;
			break;
		case	__sub:
			RD -= rbvv;
			break;
		case	__sbc:
			RD -= (rbvv + CF);
			break;
	}
	
	if(0)
	{
		switch(operation)
		{
			case	__add:
			case	__adc:
				alu_flag_fn_add(vm, RD, RA, rbvv);
				break;
			case	__and:
			case	__bic:
			case	__eor:
			case	__orr:
				alu_flag_fn_nz(vm, RD);
				break;
			case	__sub:
			case	__sbc:
				alu_flag_fn_sub(vm, RD, RA, rbvv);
				break;
		}
	}
}

enum {
	__asr,
	__lsl,
	__lsr,
	__rol,
	__ror,
};

static void alu_shift(_PASS_VM, _PASS_INST, uint8_t shift_type, uint8_t count)
{
	switch(shift_type)
	{
		case	__asr:
			RD = _ASR(RA, count);
			break;
		case	__lsl:
			RD = _LSL(RA, count);
			break;
		case	__lsr:
			RD = _LSR(RA, count);
			break;
		case	__rol:
			RD = _rol(RA, count);
			break;
		case	__ror:
			RD = _ror(RA, count);
			break;
	}
}

#define INST_ESAC_LIST \
	INST_ESAC(nop, xx_xx_xx, /* nop */) \
	\
	INST_ESAC(add, rd_ra_rb, alu(vm, inst, __add, RB)) \
	INST_ESAC(addi, rd_ra_vv, alu(vm, inst, __add, VV)) \
	INST_ESAC(adc, rd_ra_rb, alu(vm, inst, __adc, RB)) \
	INST_ESAC(adci, rd_ra_vv, alu(vm, inst, __adc, VV)) \
	INST_ESAC(and, rd_ra_rb, alu(vm, inst, __and, RB)) \
	INST_ESAC(asr, rd_ra_rb, alu_shift(vm, inst, __asr, RB)) \
	INST_ESAC(asri, rd_ra_vv, alu_shift(vm, inst, __asr, VV)) \
	INST_ESAC(bic, rd_ra_rb, alu(vm, inst, __bic, ~RB)) \
	INST_ESAC(bclr, rd_ra_vv, RD = RA & ~(_LSL(1, VV))) \
	INST_ESAC(bset, rd_ra_vv, RD = RA | (_LSL(1, VV))) \
	INST_ESAC(cmp, ra_rb_zz, RD = RA - RB; alu_flag_fn_sub(vm, RD, RA, RB)) \
	INST_ESAC(com, rd_ra_zz, RD = ~RA) \
	INST_ESAC(div, rd_ra_rb, alu(vm, inst, __div, RB)) \
	INST_ESAC(divmod, rd_ra_rb_rc, RD = RA / RB; RFV(RCr) = RA % RB) \
	INST_ESAC(eor, rd_ra_rb, alu(vm, inst, __eor, RB)) \
	INST_ESAC(imac, rd_ra_rb_rc, RD = (RA * RB) + RC) \
	INST_ESAC(lsl, rd_ra_rb, alu_shift(vm, inst, __lsl, RB)) \
	INST_ESAC(lsli, rd_ra_vv, alu_shift(vm, inst, __lsl, VV)) \
	INST_ESAC(lsr, rd_ra_rb, alu_shift(vm, inst, __lsr, RB)) \
	INST_ESAC(lsri, rd_ra_vv, alu_shift(vm, inst, __lsr, VV)) \
	INST_ESAC(maxu, rd_ra_rb, RD = MAX((unsigned)RA, (unsigned)RB)) \
	INST_ESAC(maxs, rd_ra_rb, RD = MAX((signed)RA, (signed)RB)) \
	INST_ESAC(mins, rd_ra_rb, RD = MIN((signed)RA, (signed)RB)) \
	INST_ESAC(minu, rd_ra_rb, RD = MIN((unsigned)RA, (unsigned)RB)) \
	INST_ESAC(mod, rd_ra_rb, alu(vm, inst, __mod, RB)) \
	INST_ESAC(mul, rd_ra_rb, alu(vm, inst, __mul, RB)) \
	INST_ESAC(muldiv, rd_ra_rb_rc, RD = (RA * RB) / RC) \
	INST_ESAC(nand, rd_ra_rb, RD = !(RA & RB)) \
	INST_ESAC(neg, rd_ra_zz, RD = -RA) \
	INST_ESAC(not, rd_ra_zz, RD = !RA) \
	INST_ESAC(or, rd_ra_rb, alu(vm, inst, __orr, RB)) \
	INST_ESAC(rol, rd_ra_rb, alu_shift(vm, inst, __rol, RB)) \
	INST_ESAC(roli, rd_ra_vv, alu_shift(vm, inst, __rol, VV)) \
	INST_ESAC(ror, rd_ra_rb, alu_shift(vm, inst, __ror, RB)) \
	INST_ESAC(rori, rd_ra_rb, alu_shift(vm, inst, __ror, VV)) \
	INST_ESAC(sub, rd_ra_rb, alu(vm, inst, __sub, RB)) \
	INST_ESAC(subi, rd_ra_vv, alu(vm, inst, __sub, VV)) \
	INST_ESAC(sbc, rd_ra_rb, alu(vm, inst, __sbc, RB)) \
	INST_ESAC(sbci, rd_ra_vv, alu(vm, inst, __sbc, VV)) \
	INST_ESAC(test, ra_rb_zz, RD = RA & RB; alu_flag_fn_nz(vm, RD)) \
	\
	INST_ESAC(bcc, ea_xx_xx, BRA_CC(!CF)) \
	INST_ESAC(bcs, ea_xx_xx, BRA_CC(CF)) \
	INST_ESAC(beq, ea_xx_xx, BRA_CC(!ZF)) \
	INST_ESAC(bmi, ea_xx_xx, BRA_CC(NF)) \
	INST_ESAC(bne, ea_xx_xx, BRA_CC(ZF)) \
	INST_ESAC(bpl, ea_xx_xx, BRA_CC(!NF)) \
	INST_ESAC(bvc, ea_xx_xx, BRA_CC(!OVF)) \
	INST_ESAC(bvs, ea_xx_xx, BRA_CC(OVF)) \
	\
	INST_ESAC(brreq, ra_rb_pcea, BRA_CC(RA == RB)) \
	INST_ESAC(brrgt, ra_rb_pcea, BRA_CC(RA > RB)) \
	INST_ESAC(brrge, ra_rb_pcea, BRA_CC(RA >= RB)) \
	INST_ESAC(brrlt, ra_rb_pcea, BRA_CC(RA < RB)) \
	INST_ESAC(brrle, ra_rb_pcea, BRA_CC(RA <= RB)) \
	INST_ESAC(brrne, ra_rb_pcea, BRA_CC(RA != RB)) \
	\
	INST_ESAC(moveq, rd_ra_rb_rc, MOV_CC(RA == RB, RC)) \
	INST_ESAC(movgt, rd_ra_rb_rc, MOV_CC(RA > RB, RC)) \
	INST_ESAC(movge, rd_ra_rb_rc, MOV_CC(RA >= RB, RC)) \
	INST_ESAC(movlt, rd_ra_rb_rc, MOV_CC(RA < RB, RC)) \
	INST_ESAC(movle, rd_ra_rb_rc, MOV_CC(RA <= RB, RC)) \
	INST_ESAC(movne, rd_ra_rb_rc, MOV_CC(RA != RB, RC)) \
	\
	INST_ESAC(bra, ea_xx_xx, PC = EA) \
	INST_ESAC(call, ea_xx_xx, CALL(EA)) \
	INST_ESAC(rts, xx_xx_xx, RTS()) \
	INST_ESAC(rti, xx_xx_xx, RTI()) \
	\
	INST_ESAC(push, ra_zz_zz, PUSH(RA)) \
	INST_ESAC(pop, rd_zz_zz, RD = POP()) \
	\
	INST_ESAC(enter, xx_xx_xx, PUSH(BP); BP = SP; SP -= IR_V24) \
	INST_ESAC(leave, xx_xx_xx, SP = BP; BP = POP(); RTS()) \
	\
	INST_ESAC(liu, rd_vv_vv, RD = IR_V24) \
	INST_ESAC(lis, rd_vv_vv, RD = IR_V24s) \
	\
	INST_ESAC(ldb, rd_raea, RD = *(uint8_t*)EA) \
	INST_ESAC(ldw, rd_raea, RD = *(uint16_t*)EA) \
	INST_ESAC(ldl, rd_raea, RD = *(uint32_t*)EA) \
	\
	INST_ESAC(ldbs, rd_raea, RD = *(int8_t*)EA) \
	INST_ESAC(ldws, rd_raea, RD = *(int16_t*)EA) \
	\
	INST_ESAC(stb, raea_rb, *(uint8_t*)EA = RB) \
	INST_ESAC(stw, raea_rb, *(uint16_t*)EA = RB) \
	INST_ESAC(stl, raea_rb, *(uint32_t*)EA = RB) \
	\
	INST_ESAC(iorb, rd_raea, RD = io_read(vm, EA, sizeof(uint8_t))) \
	INST_ESAC(iorw, rd_raea, RD = io_read(vm, EA, sizeof(uint16_t))) \
	INST_ESAC(iorl, rd_raea, RD = io_read(vm, EA, sizeof(uint32_t))) \
	\
	INST_ESAC(iorbs, rd_raea, RD = (int8_t)io_read(vm, EA, sizeof(uint8_t))) \
	INST_ESAC(iorws, rd_raea, RD = (int16_t)io_read(vm, EA, sizeof(uint16_t))) \
	\
	INST_ESAC(iowb, raea_rb, io_write(vm, EA, RB, sizeof(uint8_t))) \
	INST_ESAC(ioww, raea_rb, io_write(vm, EA, RB, sizeof(uint16_t))) \
	INST_ESAC(iowl, raea_rb, io_write(vm, EA, RB, sizeof(uint32_t))) \
	\
	INST_ESAC(sextb, rd_ra_zz, RD = (int8_t)RA) \
	INST_ESAC(sextw, rd_ra_zz, RD = (int16_t)RA) \
	\
	INST_ESAC(swap16, rd_ra_zz, RD = __builtin_bswap16(RA)) \
	INST_ESAC(swap32, rd_ra_zz, RD = __builtin_bswap32(RA)) \
	\
	INST_ESAC(syscall1, rd_ra_zz,/*RD = host_syscall1(vm, RA)*/) \
	INST_ESAC(syscall2, rd_ra_rb,/*RD = host_syscall1(vm, RA, RB)*/) \
	INST_ESAC(syscall3, rd_ra_rb_rc,/*RD = host_syscall1(vm, RA, RB, RC)*/)
