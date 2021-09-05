#if 1
	#define TRACE(_f, args...) \
		printf("%s:%04u:(0x%08x, 0x%08x):0x%016llx:: " _f "\n", \
			__FUNCTION__, __LINE__, PC, (PC - (int)&vm->rom), vm->cycle, ##args);
#else
	#define TRACE(_f, args...)
#endif
