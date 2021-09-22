enum {
	_brrcc,
	_movcc
};

enum {
	_rrcc_eq,
	_rrcc_ge,
	_rrcc_gt,
	_rrcc_le,
	_rrcc_lt,
	_rrcc_ne
};

#define RRCC_ESAC(_rrcc_cc, _test) \
	ESAC_ACTION(_rrcc_##_rrcc_cc, cc_pass = (_test))

static inline void rrcc(vm_p vm, int test, int operation)
{
	int cc_pass;
	
	switch(test)
	{
		RRCC_ESAC(eq, vR(A) == vR(B));
		RRCC_ESAC(ge, vR(A) >= vR(B));
		RRCC_ESAC(gt, vR(A) > vR(B));
		RRCC_ESAC(le, vR(A) <= vR(B));
		RRCC_ESAC(lt, vR(A) < vR(B));
		RRCC_ESAC(ne, vR(A) != vR(B));
	}
	
	switch(operation)
	{
		case	_brrcc:
			BRA_CC(cc_pass);
			break;
		case	_movcc:
			MOV_CC(cc_pass, vR(C));
			break;
	}
}

#define RRCC(_test, _operation) \
	rrcc(vm, _rrcc_##_test, _##_operation##cc)
