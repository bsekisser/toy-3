enum {
	alu_rd_rs_i,
	alu_rd_rs_rs,
	shift_rd_rs_rsa,
	ld_mem_rd_rsea,
	st_mem_rdea_rs,
	ld_io_rd_rsea,
	st_io_rdea_rs,
	bra_ea,
	bra_cc_ea,
	call_ea,
};

#define INST_ENUM(_enum) \
	_inst_enum_##_enum##_k

#define ESAC_ACTION(_esac, _action) \
	case _esac: \
		_action; \
		break;

#define INST_ESAC_0_0_LIST \
	INST_ESAC(lui, type_rd_ra_i, LUI_ORI_NOP(), nop, wb_rd) \
	\
	INST_ESAC(bcc, type_pcea, BRA_CC(!CF), nop, nop) \
	INST_ESAC(bcs, type_pcea, BRA_CC(CF), nop, nop) \
	INST_ESAC(beq, type_pcea, BRA_CC(!ZF), nop, nop) \
	INST_ESAC(bmi, type_pcea, BRA_CC(NF), nop, nop) \
	INST_ESAC(bne, type_pcea, BRA_CC(ZF), nop, nop) \
	INST_ESAC(bpl, type_pcea, BRA_CC(!NF), nop, nop) \
	INST_ESAC(bvc, type_pcea, BRA_CC(!OVF), nop, nop) \
	INST_ESAC(bvs, type_pcea, BRA_CC(OVF), nop, nop) \
	\
	INST_ESAC(bra, type_pcea, PC = EA, nop, nop) \
	INST_ESAC(call, type_pcea, CALL(EA), nop, nop)

#define INST_ESAC_0_1_LIST \
	INST_ESAC(addi, type_rd_ra_i, ALU_I(add), nop, wb_rd) \
	INST_ESAC(addiu, type_rd_ra_i, ALU_Iu(add), nop, wb_rd) \
	INST_ESAC(adci, type_rd_ra_i, ALU_I(adc), nop, wb_rd) \
	INST_ESAC(subi, type_rd_ra_i, ALU_I(sub), nop, wb_rd) \
	INST_ESAC(subiu, type_rd_ra_i, ALU_Iu(sub), nop, wb_rd) \
	INST_ESAC(sbci, type_rd_ra_i, ALU_I(sbc), nop, wb_rd) \
	\
	INST_ESAC(andi, type_rd_ra_i, ALU_Iu(and), nop, wb_rd) \
	INST_ESAC(orri, type_rd_ra_i, ALU_Iu(orr), nop, wb_rd) \
	INST_ESAC(xori, type_rd_ra_i, ALU_Iu(xor), nop, wb_rd) \
	\
	INST_ESAC(bclr, type_rd_ra_i, ALU_RBVV(bic, _LSL(1, VV), 0), nop, wb_rd) \
	INST_ESAC(bset, type_rd_ra_i, ALU_RBVV(orr, _LSL(1, VV), 0), nop, wb_rd) \
	\
	INST_ESAC(ldb, type_rd_raea, NOP(), ma_ld8u, wb_rd) \
	INST_ESAC(ldw, type_rd_raea, NOP(), ma_ld16u, wb_rd) \
	INST_ESAC(ldl, type_rd_raea, NOP(), ma_ld32, wb_rd) \
	\
	INST_ESAC(ldbs, type_rd_raea, NOP(), ma_ld8s, wb_rd) \
	INST_ESAC(ldws, type_rd_raea, NOP(), ma_ld16s, wb_rd) \
	\
	INST_ESAC(stb, type_rdea_ra, NOP(), ma_st8, nop) \
	INST_ESAC(stw, type_rdea_ra, NOP(), ma_st16, nop) \
	INST_ESAC(stl, type_rdea_ra, NOP(), ma_st32, nop) \
	\
	INST_ESAC(iorb, type_rd_raea, NOP(), io_ld8u, wb_rd) \
	INST_ESAC(iorw, type_rd_raea, NOP(), io_ld16u, wb_rd) \
	INST_ESAC(iorl, type_rd_raea, NOP(), io_ld32, wb_rd) \
	\
	INST_ESAC(iorbs, type_rd_raea, NOP(), io_ld8s, wb_rd) \
	INST_ESAC(iorws, type_rd_raea, NOP(), io_ld16s, wb_rd) \
	\
	INST_ESAC(iowb, type_rdea_ra, NOP(), io_st8, nop) \
	INST_ESAC(ioww, type_rdea_ra, NOP(), io_st16, nop) \
	INST_ESAC(iowl, type_rdea_ra, NOP(), io_st32, nop) \
	\
	INST_ESAC(brreq, type_ra_rb_pcea, RRCC(eq, brr), nop, nop) \
	INST_ESAC(brrgt, type_ra_rb_pcea, RRCC(gt, brr), nop, nop) \
	INST_ESAC(brrge, type_ra_rb_pcea, RRCC(ge, brr), nop, nop) \
	INST_ESAC(brrlt, type_ra_rb_pcea, RRCC(lt, brr), nop, nop) \
	INST_ESAC(brrle, type_ra_rb_pcea, RRCC(le, brr), nop, nop) \
	INST_ESAC(brrne, type_ra_rb_pcea, RRCC(ne, brr), nop, nop) \
	\
	INST_ESAC(dbeq, type_rd_ra_pcea, DBRA_CC(vR(D) == 0), nop, wb_rd) \
	INST_ESAC(dbnz, type_rd_ra_pcea, DBRA_CC((vR(D) > 0) && (vR(D) != 0)), nop, wb_rd)

#define INST_ESAC_0_LIST \
	INST_ESAC_0_0_LIST \
	INST_ESAC_0_1_LIST

#define INST_ESAC_1_0_LIST \
	INST_ESAC(add, type_rd_ra_rb, ALU(add), nop, wb_rd) \
	INST_ESAC(adc, type_rd_ra_rb, ALU(adc), nop, wb_rd) \
	INST_ESAC(sub, type_rd_ra_rb, ALU(sub), nop, wb_rd) \
	INST_ESAC(sbc, type_rd_ra_rb, ALU(sbc), nop, wb_rd) \
	\
	INST_ESAC(and, type_rd_ra_rb, ALU(and), nop, wb_rd) \
	INST_ESAC(bic, type_rd_ra_rb, ALU(bic), nop, wb_rd) \
	INST_ESAC(orr, type_rd_ra_rb, ALU(orr), nop, wb_rd) \
	INST_ESAC(xor, type_rd_ra_rb, ALU(xor), nop, wb_rd) \
	\
	INST_ESAC(asr, type_rd_ra_rbsa, SHIFT(asr), nop, wb_rd) \
	INST_ESAC(lsl, type_rd_ra_rbsa, SHIFT(lsl), nop, wb_rd) \
	INST_ESAC(lsr, type_rd_ra_rbsa, SHIFT(lsr), nop, wb_rd) \
	\
	INST_ESAC(rol, type_rd_ra_rbsa, SHIFT(rol), nop, wb_rd) \
	INST_ESAC(ror, type_rd_ra_rbsa, SHIFT(ror), nop, wb_rd) \
	INST_ESAC(rlc, type_rd_ra_rbsa, SHIFT(rlc), nop, wb_rd) \
	INST_ESAC(rrc, type_rd_ra_rbsa, SHIFT(rrc), nop, wb_rd) \
	\
	INST_ESAC(enter, type_u, PUSH(BP); BP = SP; SP -= VV, nop, nop)

#define INST_ESAC_1_1_LIST \
	INST_ESAC(nand, type_rd_ra_rb, vR(D) = !(vR(A) & vR(B)), nop, wb_rd) \
	INST_ESAC(xnor, type_rd_ra_rb, vR(D) = !(vR(A) ^ vR(B)), nop, wb_rd) \
	INST_ESAC(nor, type_rd_ra_rb, vR(D) = !(vR(A) | vR(B)), nop, wb_rd) \
	\
	INST_ESAC(div, type_rd_ra_rb, ALU(div), nop, wb_rd) \
	INST_ESAC(mod, type_rd_ra_rb, ALU(mod), nop, wb_rd) \
	INST_ESAC(mul, type_rd_ra_rb, ALU(mul), nop, wb_rd) \
	\
	INST_ESAC(leave, nop, SP = BP; BP = POP(); RTS(), nop, nop) \
	INST_ESAC(push, type_ra, PUSH(vR(A)), nop, nop) \
	INST_ESAC(pop, type_rd, vR(D) = POP(), nop, wb_rd)

#define INST_ESAC_1_2_LIST \
	INST_ESAC(muladd, type_rd_ra_rb_rc, vR(D) = (vR(A) * vR(B)) + vR(C), nop, wb_rd) \
	INST_ESAC(muldiv, type_rd_ra_rb_rc, vR(D) = (vR(A) * vR(B)) / vR(C), nop, wb_rd) \
	INST_ESAC(mulsub, type_rd_ra_rb_rc, vR(D) = (vR(A) * vR(B)) - vR(C), nop, wb_rd) \
	\
	INST_ESAC(mult, type_ra_rb, ALU_Q(mult), nop, nop) \
	INST_ESAC(multu, type_ra_rb, ALU_Q(multu), nop, nop) \
	INST_ESAC(divu, type_ra_rb, ALU_Q(divu), nop, nop) \
	INST_ESAC(divmod, type_ra_rb, ALU_Q(divmod), nop, nop) \
	\
	INST_ESAC(madd, type_ra_rb, ALU_Q(madd), nop, nop) \
	INST_ESAC(maddu, type_ra_rb, ALU_Q(maddu), nop, nop) \
	INST_ESAC(msub, type_ra_rb, ALU_Q(msub), nop, nop) \
	INST_ESAC(msubu, type_ra_rb, ALU_Q(msubu), nop, nop) \
	\
	INST_ESAC(mfspr, type_rd_a, vR(D) = SPR(rR(A)), nop, wb_rd) \
	INST_ESAC(mtspr, type_ra_b, SPR(rR(B)) = vR(A), nop, nop) \
	\
	INST_ESAC(maxu, type_rd_ra_rb, vR(D) = MAX((unsigned)vR(A), (unsigned)vR(B)), nop, wb_rd) \
	INST_ESAC(maxs, type_rd_ra_rb, vR(D) = MAX((signed)vR(A), (signed)vR(B)), nop, wb_rd) \
	INST_ESAC(mins, type_rd_ra_rb, vR(D) = MIN((signed)vR(A), (signed)vR(B)), nop, wb_rd) \
	INST_ESAC(minu, type_rd_ra_rb, vR(D) = MIN((unsigned)vR(A), (unsigned)vR(B)), nop, wb_rd) \
	\
	INST_ESAC(moveq, type_rd_ra_rb_rc, RRCC(eq, mov), nop, wb_rd) \
	INST_ESAC(movge, type_rd_ra_rb_rc, RRCC(ge, mov), nop, wb_rd) \
	INST_ESAC(movgt, type_rd_ra_rb_rc, RRCC(gt, mov), nop, wb_rd) \
	INST_ESAC(movle, type_rd_ra_rb_rc, RRCC(le, mov), nop, wb_rd) \
	INST_ESAC(movlt, type_rd_ra_rb_rc, RRCC(lt, mov), nop, wb_rd) \
	INST_ESAC(movne, type_rd_ra_rb_rc, RRCC(ne, mov), nop, wb_rd)

#define INST_ESAC_1_LIST \
	INST_ESAC_1_0_LIST \
	INST_ESAC_1_1_LIST \
	INST_ESAC_1_2_LIST \
	INST_XXX

/*
 * pseudo operations
 * 
 * cmp ra?, rb?		-->		sub rd0, ra?, rb?
 * test ra?, rb?	-->		and rd0, ra?, rb?
 * 
 * com rd?, s?		-->		bic rd?, rs0, rs?
 * mov rd?, s?		-->		orr rd?, rs0, rs?
 * neg rd?, s?		-->		sub rd?, rs0, rs?
 * not rd?, s?		-->		nor rd?, rs0, rs?
 * 
 */

/*
	nop rd0, rs0	--	0000 0000 | 0000 0000 | 0000 0000 | 0000 0000
	lui rd?, rs0	--	iiii iiii | iiii iiii | 0000 0ddd | dd00 0000
	ori rd?, rs0	--	iiii iiii | iiii iiii | aaaa addd | dd00 0000
*/

#define INST_ESAC_LIST \
	INST_ESAC_0_LIST \
	INST_ESAC_1_LIST

#define INST_XXX \
	INST_ESAC(bmas, type_rd_ra_rb_rc, ALU(bmas), nop, wb_rd) \
	\
	INST_ESAC(ins, type_rd_ra_b_c, vR(D) = mlBFINS(GPR(rR(D)), vR(A), rR(B), rR(C)), nop, wb_rd) \
	INST_ESAC(ext, type_rd_ra_b_c, vR(D) = mlBFEXT(vR(A), rR(B), rR(C)), nop, wb_rd) \
	\
	INST_ESAC(rts, nop, RTS(), nop, nop) \
	INST_ESAC(rti, nop, RTI(), nop, nop) \
	\
	INST_ESAC(sextb, type_rd_ra, vR(D) = (int8_t)vR(A), nop, wb_rd) \
	INST_ESAC(sextw, type_rd_ra, vR(D) = (int16_t)vR(A), nop, wb_rd) \
	\
	INST_ESAC(swap16, type_rd_ra, vR(D) = __builtin_bswap16(vR(A)), nop, wb_rd) \
	INST_ESAC(swap32, type_rd_ra, vR(D) = __builtin_bswap32(vR(A)), nop, wb_rd) \
	\
	INST_ESAC(syscall1, type_rd_ra,/*vR(D) = host_syscall1(vm, vR(A))*/, nop, wb_rd) \
	INST_ESAC(syscall2, type_rd_ra_rb,/*vR(D) = host_syscall2(vm, vR(A), vR(B))*/, nop, wb_rd) \
	INST_ESAC(syscall3, type_rd_ra_rb_rc,/*vR(D) = host_syscall3(vm, vR(A), vR(B), vR(C))*/, nop, wb_rd)
