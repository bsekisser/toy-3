#define INST_ESAC_LIST \
	INST_ESAC(nop, xx_xx_xx, /* nop */) \
	\
	INST_ESAC(add, rd_ra_rb, ALU(add, RB, 0)) \
	INST_ESAC(addi, rd_ra_vv, ALU(add, VV, 0)) \
	INST_ESAC(adc, rd_ra_rb, ALU(adc, RB, 0)) \
	INST_ESAC(adci, rd_ra_vv, ALU(adc, VV, 0)) \
	INST_ESAC(and, rd_ra_rb, ALU(and, RB, 0)) \
	INST_ESAC(andi, rd_ra_rb, ALU(and, VV, 0)) \
	INST_ESAC(asr, rd_ra_rb, ALU_SHIFT(asr, RB)) \
	INST_ESAC(asri, rd_ra_vv, ALU_SHIFT(asr, VV)) \
	INST_ESAC(bic, rd_ra_rb, ALU(bic, RB, 0)) \
	INST_ESAC(bclr, rd_ra_vv, ALU(bic, _LSL(1, VV), 0)) \
	INST_ESAC(bset, rd_ra_vv, ALU(orr, _LSL(1, VV), 0)) \
	INST_ESAC(cmp, ra_rb_zz, ALU(sub, RB, 1)) \
	INST_ESAC(cmpi, ra_rb_vv, ALU(sub, VV, 1)) \
	INST_ESAC(com, rd_ra_zz, RD = ~RA) \
	INST_ESAC(div, rd_ra_rb, ALU(div, RB, 0)) \
	INST_ESAC(divmod, rdrc_ra_rb, RD = RA / RB; RC = RA % RB) \
	INST_ESAC(eor, rd_ra_rb, ALU(eor, RB, 0)) \
	INST_ESAC(imac, rd_ra_rb_rc, RD = (RA * RB) + RC) \
	INST_ESAC(lsl, rd_ra_rb, ALU_SHIFT(lsl, RB)) \
	INST_ESAC(lsli, rd_ra_vv, ALU_SHIFT(lsl, VV)) \
	INST_ESAC(lsr, rd_ra_rb, ALU_SHIFT(lsr, RB)) \
	INST_ESAC(lsri, rd_ra_vv, ALU_SHIFT(lsr, VV)) \
	INST_ESAC(maxu, rd_ra_rb, RD = MAX((unsigned)RA, (unsigned)RB)) \
	INST_ESAC(maxs, rd_ra_rb, RD = MAX((signed)RA, (signed)RB)) \
	INST_ESAC(mins, rd_ra_rb, RD = MIN((signed)RA, (signed)RB)) \
	INST_ESAC(minu, rd_ra_rb, RD = MIN((unsigned)RA, (unsigned)RB)) \
	INST_ESAC(mod, rd_ra_rb, ALU(mod, RB, 0)) \
	INST_ESAC(mov, rd_ra_zz, ALU(mov, 0, 0)) \
	INST_ESAC(mul, rd_ra_rb, ALU(mul, RB, 0)) \
	INST_ESAC(muldiv, rd_ra_rb_rc, RD = (RA * RB) / RC) \
	INST_ESAC(nand, rd_ra_rb, RD = !(RA & RB)) \
	INST_ESAC(neg, rd_ra_zz, RD = -RA) \
	INST_ESAC(not, rd_ra_zz, RD = !RA) \
	INST_ESAC(orr, rd_ra_rb, ALU(orr, RB, 0)) \
	INST_ESAC(rlc, rd_ra_vv, ALU_SHIFT(rlc, RB)) \
	INST_ESAC(rlci, rd_ra_vv, ALU_SHIFT(rlc, VV)) \
	INST_ESAC(rol, rd_ra_rb, ALU_SHIFT(rol, RB)) \
	INST_ESAC(roli, rd_ra_vv, ALU_SHIFT(rol, VV)) \
	INST_ESAC(ror, rd_ra_rb, ALU_SHIFT(ror, RB)) \
	INST_ESAC(rori, rd_ra_rb, ALU_SHIFT(ror, VV)) \
	INST_ESAC(rrc, rd_ra_vv, ALU_SHIFT(rrc, RB)) \
	INST_ESAC(rrci, rd_ra_vv, ALU_SHIFT(rrc, VV)) \
	INST_ESAC(sub, rd_ra_rb, ALU(sub, RB, 0)) \
	INST_ESAC(subi, rd_ra_vv, ALU(sub, VV, 0)) \
	INST_ESAC(sbc, rd_ra_rb, ALU(sbc, RB, 0)) \
	INST_ESAC(sbci, rd_ra_vv, ALU(sbc, VV, 0)) \
	INST_ESAC(test, ra_rb_zz, ALU(and, RB, 1)) \
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
	INST_ESAC(dbeq, rd_ra_pcea, DBRA_CC(RD == 0)) \
	INST_ESAC(dbnz, rd_ra_pcea, DBRA_CC((RD > 0) && (RD != 0))) \
	\
	INST_ESAC(moveq, rd_ra_rb_rc, MOV_CC(RA == RB, RC)) \
	INST_ESAC(movgt, rd_ra_rb_rc, MOV_CC(RA > RB, RC)) \
	INST_ESAC(movge, rd_ra_rb_rc, MOV_CC(RA >= RB, RC)) \
	INST_ESAC(movlt, rd_ra_rb_rc, MOV_CC(RA < RB, RC)) \
	INST_ESAC(movle, rd_ra_rb_rc, MOV_CC(RA <= RB, RC)) \
	INST_ESAC(movne, rd_ra_rb_rc, MOV_CC(RA != RB, RC)) \
	\
	INST_ESAC(bra, ea_xx_xx, TRACE("bra 0x%08x -- 0x%08x", EA, IR_V24s); PC = EA) \
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
	INST_ESAC(liu, rd_vv_vv, RD = VV) \
	INST_ESAC(lis, rd_vv_vv, RD = IR_V20s) \
	\
	INST_ESAC(ldb, rd_raea, INST_MA_Ru(sizeof(uint8_t))) \
	INST_ESAC(ldw, rd_raea, INST_MA_Ru(sizeof(uint16_t))) \
	INST_ESAC(ldl, rd_raea, INST_MA_Ru(sizeof(uint32_t))) \
	\
	INST_ESAC(ldbs, rd_raea, INST_MA_Rs(sizeof(int8_t))) \
	INST_ESAC(ldws, rd_raea, INST_MA_Rs(sizeof(int16_t))) \
	\
	INST_ESAC(stb, rdea_ra, INST_MA_W(sizeof(uint8_t))) \
	INST_ESAC(stw, rdea_ra, INST_MA_W(sizeof(uint16_t))) \
	INST_ESAC(stl, rdea_ra, INST_MA_W(sizeof(uint32_t))) \
	\
	INST_ESAC(iorb, rd_raea, INST_IO_Ru(sizeof(uint8_t))) \
	INST_ESAC(iorw, rd_raea, INST_IO_Ru(sizeof(uint16_t))) \
	INST_ESAC(iorl, rd_raea, INST_IO_Ru(sizeof(uint32_t))) \
	\
	INST_ESAC(iorbs, rdea_ra, INST_IO_Rs(sizeof(int8_t))) \
	INST_ESAC(iorws, rdea_ra, INST_IO_Rs(sizeof(int16_t))) \
	\
	INST_ESAC(iowb, rdea_ra, INST_IO_W(sizeof(uint8_t))) \
	INST_ESAC(ioww, rdea_ra, INST_IO_W(sizeof(uint16_t))) \
	INST_ESAC(iowl, rdea_ra, INST_IO_W(sizeof(uint32_t))) \
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
