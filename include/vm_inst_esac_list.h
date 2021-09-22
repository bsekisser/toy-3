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
	INST_ESAC(lui, type_i, rd_vra, LUI_ORI_NOP(), nop, wb_rd) \
	\
	INST_ESAC(bcc, type_i, pcea, BRA_CC(!CF), nop, nop) \
	INST_ESAC(bcs, type_i, pcea, BRA_CC(CF), nop, nop) \
	INST_ESAC(beq, type_i, pcea, BRA_CC(!ZF), nop, nop) \
	INST_ESAC(bmi, type_i, pcea, BRA_CC(NF), nop, nop) \
	INST_ESAC(bne, type_i, pcea, BRA_CC(ZF), nop, nop) \
	INST_ESAC(bpl, type_i, pcea, BRA_CC(!NF), nop, nop) \
	INST_ESAC(bvc, type_i, pcea, BRA_CC(!OVF), nop, nop) \
	INST_ESAC(bvs, type_i, pcea, BRA_CC(OVF), nop, nop) \
	\
	INST_ESAC(bra, type_i, pcea, PC = EA, nop, nop) \
	INST_ESAC(call, type_i, pcea, CALL(EA), nop, nop)

#define INST_ESAC_0_1_LIST \
	INST_ESAC(addi, type_i, rd_vra, ALU_I(add), nop, wb_rd) \
	INST_ESAC(addiu, type_i, rd_vra, ALU_Iu(add), nop, wb_rd) \
	INST_ESAC(adci, type_i, rd_vra, ALU_I(adc), nop, wb_rd) \
	INST_ESAC(subi, type_i, rd_vra, ALU_I(sub), nop, wb_rd) \
	INST_ESAC(subiu, type_i, rd_vra, ALU_Iu(sub), nop, wb_rd) \
	INST_ESAC(sbci, type_i, rd_vra, ALU_I(sbc), nop, wb_rd) \
	\
	INST_ESAC(andi, type_i, rd_vra, ALU_Iu(and), nop, wb_rd) \
	INST_ESAC(orri, type_i, rd_vra, ALU_Iu(orr), nop, wb_rd) \
	INST_ESAC(xori, type_i, rd_vra, ALU_Iu(xor), nop, wb_rd) \
	\
	INST_ESAC(bclr, type_i, rd_vra, ALU_RBVV(bic, _LSL(1, VV), 0), nop, wb_rd) \
	INST_ESAC(bset, type_i, rd_vra, ALU_RBVV(orr, _LSL(1, VV), 0), nop, wb_rd) \
	\
	INST_ESAC(ldb, type_i, rd_vraea, NOP(), ma_ld8u, wb_rd) \
	INST_ESAC(ldw, type_i, rd_vraea, NOP(), ma_ld16u, wb_rd) \
	INST_ESAC(ldl, type_i, rd_vraea, NOP(), ma_ld32, wb_rd) \
	\
	INST_ESAC(ldbs, type_i, rd_vraea, NOP(), ma_ld8s, wb_rd) \
	INST_ESAC(ldws, type_i, rd_vraea, NOP(), ma_ld16s, wb_rd) \
	\
	INST_ESAC(stb, type_i, vrdea_vra, NOP(), ma_st8, nop) \
	INST_ESAC(stw, type_i, vrdea_vra, NOP(), ma_st16, nop) \
	INST_ESAC(stl, type_i, vrdea_vra, NOP(), ma_st32, nop) \
	\
	INST_ESAC(iorb, type_i, rd_vraea, NOP(), io_ld8u, wb_rd) \
	INST_ESAC(iorw, type_i, rd_vraea, NOP(), io_ld16u, wb_rd) \
	INST_ESAC(iorl, type_i, rd_vraea, NOP(), io_ld32, wb_rd) \
	\
	INST_ESAC(iorbs, type_i, rd_vraea, NOP(), io_ld8s, wb_rd) \
	INST_ESAC(iorws, type_i, rd_vraea, NOP(), io_ld16s, wb_rd) \
	\
	INST_ESAC(iowb, type_i, vrdea_vra, NOP(), io_st8, nop) \
	INST_ESAC(ioww, type_i, vrdea_vra, NOP(), io_st16, nop) \
	INST_ESAC(iowl, type_i, vrdea_vra, NOP(), io_st32, nop) \
	\
	INST_ESAC(brreq, type_i, vra_vrb_pcea, RRCC(eq, brr), nop, nop) \
	INST_ESAC(brrgt, type_i, vra_vrb_pcea, RRCC(gt, brr), nop, nop) \
	INST_ESAC(brrge, type_i, vra_vrb_pcea, RRCC(ge, brr), nop, nop) \
	INST_ESAC(brrlt, type_i, vra_vrb_pcea, RRCC(lt, brr), nop, nop) \
	INST_ESAC(brrle, type_i, vra_vrb_pcea, RRCC(le, brr), nop, nop) \
	INST_ESAC(brrne, type_i, vra_vrb_pcea, RRCC(ne, brr), nop, nop) \
	\
	INST_ESAC(dbeq, type_i, rd_vra_pcea, DBRA_CC(vR(D) == 0), nop, wb_rd) \
	INST_ESAC(dbnz, type_i, rd_vra_pcea, DBRA_CC((vR(D) > 0) && (vR(D) != 0)), nop, wb_rd)

#define INST_ESAC_0_LIST \
	INST_ESAC_0_0_LIST \
	INST_ESAC_0_1_LIST

#define INST_ESAC_1_0_LIST \
	INST_ESAC(add, type_r, rd_vra_vrb, ALU(add), nop, wb_rd) \
	INST_ESAC(adc, type_r, rd_vra_vrb, ALU(adc), nop, wb_rd) \
	INST_ESAC(sub, type_r, rd_vra_vrb, ALU(sub), nop, wb_rd) \
	INST_ESAC(sbc, type_r, rd_vra_vrb, ALU(sbc), nop, wb_rd) \
	\
	INST_ESAC(and, type_r, rd_vra_vrb, ALU(and), nop, wb_rd) \
	INST_ESAC(bic, type_r, rd_vra_vrb, ALU(bic), nop, wb_rd) \
	INST_ESAC(orr, type_r, rd_vra_vrb, ALU(orr), nop, wb_rd) \
	INST_ESAC(xor, type_r, rd_vra_vrb, ALU(xor), nop, wb_rd) \
	\
	INST_ESAC(asr, type_r, rd_vra_vrbsa, SHIFT(asr), nop, wb_rd) \
	INST_ESAC(lsl, type_r, rd_vra_vrbsa, SHIFT(lsl), nop, wb_rd) \
	INST_ESAC(lsr, type_r, rd_vra_vrbsa, SHIFT(lsr), nop, wb_rd) \
	\
	INST_ESAC(rol, type_r, rd_vra_vrbsa, SHIFT(rol), nop, wb_rd) \
	INST_ESAC(ror, type_r, rd_vra_vrbsa, SHIFT(ror), nop, wb_rd) \
	INST_ESAC(rlc, type_r, rd_vra_vrbsa, SHIFT(rlc), nop, wb_rd) \
	INST_ESAC(rrc, type_r, rd_vra_vrbsa, SHIFT(rrc), nop, wb_rd) \
	\
	INST_ESAC(enter, type_u, nop, PUSH(BP); BP = SP; SP -= VV, nop, nop)

#define INST_ESAC_1_1_LIST \
	INST_ESAC(nand, type_r, rd_vra_vrb, vR(D) = !(vR(A) & vR(B)), nop, wb_rd) \
	INST_ESAC(nor, type_r, rd_vra_vrb, vR(D) = !(vR(A) | vR(B)), nop, wb_rd) \
	INST_ESAC(xnor, type_r, rd_vra_vrb, vR(D) = !(vR(A) ^ vR(B)), nop, wb_rd) \
	\
	INST_ESAC(div, type_r, rd_vra_vrb, ALU(div), nop, wb_rd) \
	INST_ESAC(mod, type_r, rd_vra_vrb, ALU(mod), nop, wb_rd) \
	INST_ESAC(mul, type_r, rd_vra_vrb, ALU(mul), nop, wb_rd) \
	\
	INST_ESAC(leave, nop, nop, SP = BP; BP = POP(); RTS(), nop, nop) \
	INST_ESAC(push, type_r, vra, PUSH(vR(A)), nop, nop) \
	INST_ESAC(pop, type_r, rd, vR(D) = POP(), nop, wb_rd)

#define INST_ESAC_1_2_LIST \
	INST_ESAC(muladd, type_r, rd_vra_vrb_vrc, vR(D) = (vR(A) * vR(B)) + vR(C), nop, wb_rd) \
	INST_ESAC(muldiv, type_r, rd_vra_vrb_vrc, vR(D) = (vR(A) * vR(B)) / vR(C), nop, wb_rd) \
	INST_ESAC(mulsub, type_r, rd_vra_vrb_vrc, vR(D) = (vR(A) * vR(B)) - vR(C), nop, wb_rd) \
	\
	INST_ESAC(mult, type_r, vra_vrb, ALU_Q(mult), nop, nop) \
	INST_ESAC(multu, type_r, vra_vrb, ALU_Q(multu), nop, nop) \
	INST_ESAC(divu, type_r, vra_vrb, ALU_Q(divu), nop, nop) \
	INST_ESAC(divmod, type_r, vra_vrb, ALU_Q(divmod), nop, nop) \
	\
	INST_ESAC(madd, type_r, vra_vrb, ALU_Q(madd), nop, nop) \
	INST_ESAC(maddu, type_r, vra_vrb, ALU_Q(maddu), nop, nop) \
	INST_ESAC(msub, type_r, vra_vrb, ALU_Q(msub), nop, nop) \
	INST_ESAC(msubu, type_r, vra_vrb, ALU_Q(msubu), nop, nop) \
	\
	INST_ESAC(mfspr, type_r, rd_ra, vR(D) = SPR(rR(A)), nop, wb_rd) \
	INST_ESAC(mtspr, type_r, rd_vra, vR(D) = vR(A), nop, wb_spr) \
	\
	INST_ESAC(maxu, type_r, rd_vra_vrb, vR(D) = MAX((unsigned)vR(A), (unsigned)vR(B)), nop, wb_rd) \
	INST_ESAC(maxs, type_r, rd_vra_vrb, vR(D) = MAX((signed)vR(A), (signed)vR(B)), nop, wb_rd) \
	INST_ESAC(mins, type_r, rd_vra_vrb, vR(D) = MIN((signed)vR(A), (signed)vR(B)), nop, wb_rd) \
	INST_ESAC(minu, type_r, rd_vra_vrb, vR(D) = MIN((unsigned)vR(A), (unsigned)vR(B)), nop, wb_rd) \
	\
	INST_ESAC(moveq, type_r, rd_vra_vrb_vrc, RRCC(eq, mov), nop, wb_rd) \
	INST_ESAC(movge, type_r, rd_vra_vrb_vrc, RRCC(ge, mov), nop, wb_rd) \
	INST_ESAC(movgt, type_r, rd_vra_vrb_vrc, RRCC(gt, mov), nop, wb_rd) \
	INST_ESAC(movle, type_r, rd_vra_vrb_vrc, RRCC(le, mov), nop, wb_rd) \
	INST_ESAC(movlt, type_r, rd_vra_vrb_vrc, RRCC(lt, mov), nop, wb_rd) \
	INST_ESAC(movne, type_r, rd_vra_vrb_vrc, RRCC(ne, mov), nop, wb_rd)

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
	INST_ESAC(bmas, type_r, rd_vra_vrb_vrc, ALU(bmas), nop, wb_rd) \
	\
	INST_ESAC(ins, type_r, rd_vra_rb_rc, vR(D) = mlBFINS(GPR(rR(D)), vR(A), rR(B), rR(C)), nop, wb_rd) \
	INST_ESAC(ext, type_r, rd_vra_rb_rc, vR(D) = mlBFEXT(vR(A), rR(B), rR(C)), nop, wb_rd) \
	\
	INST_ESAC(rts, nop, nop, RTS(), nop, nop) \
	INST_ESAC(rti, nop, nop, RTI(), nop, nop) \
	\
	INST_ESAC(sextb, type_r, rd_vra, vR(D) = (int8_t)vR(A), nop, wb_rd) \
	INST_ESAC(sextw, type_r, rd_vra, vR(D) = (int16_t)vR(A), nop, wb_rd) \
	\
	INST_ESAC(swap16, type_r, rd_vra, vR(D) = __builtin_bswap16(vR(A)), nop, wb_rd) \
	INST_ESAC(swap32, type_r, rd_vra, vR(D) = __builtin_bswap32(vR(A)), nop, wb_rd) \
	\
	INST_ESAC(syscall1, type_r, rd_vra,/*vR(D) = host_syscall1(vm, vR(A))*/, nop, wb_rd) \
	INST_ESAC(syscall2, type_r, rd_vra_vrb,/*vR(D) = host_syscall2(vm, vR(A), vR(B))*/, nop, wb_rd) \
	INST_ESAC(syscall3, type_r, rd_vra_vrb_vrc,/*vR(D) = host_syscall3(vm, vR(A), vR(B), vR(C))*/, nop, wb_rd)
