#if _TRACE_WRITEBACK
	#define TRACE_WB(_f, args...) TRACE(_f, ##args)
#else
	#define TRACE_WB(_f, args...)
#endif

static void vm_step_4_writeback_nop(vm_p vm)
{
	TRACE_WB();

//	TAILCALL_NEXT();
}

static void vm_step_4_writeback_wb_rd(vm_p vm)
{
	TRACE_WB("WBd -- r%0u == 0x%08x", rR(D), vR(D));

	if(rR(D))
		GPR(rR(D)) = vR(D);

//	TAILCALL_NEXT();
}

/*static void vm_step_4_writeback_wb_rdrc(vm_p vm)
{
	vm_step_4_writeback_wb_rd(vm, inst);

	TRACE_WB("WBc -- r%0u == 0x%08x", RCr, RC);

	if(rR(C))
		GPR(rR(C)) = vR(C);
}*/
