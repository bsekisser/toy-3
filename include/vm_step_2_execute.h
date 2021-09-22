#if _TRACE_EXECUTE
	#define TRACE_EXECUTE(_f, args...) TRACE(_f, ##args)
#else
	#define TRACE_EXECUTE(_f, args...)
#endif


#undef INST_ESAC
#define INST_ESAC(_esac, _decode, _action, _mem, _wb) \
	static void vm_step_2_execute_##_esac(vm_p vm) \
	{ \
		TRACE_EXECUTE(); \
	\
		IF_PIPELINE(0, vm_step_1_decode_##_decode(vm)); \
		\
		do { _action; }while(0); \
	\
		IF_PIPELINE(0, vm_step_3_##_mem(vm)); \
		IF_PIPELINE(0, vm_step_4_writeback_##_wb(vm)); \
	\
		TAILCALL_NEXT(); \
	}

INST_ESAC_LIST
