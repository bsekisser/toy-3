#define TRACE(_f, args...) \
	printf("%s:(0x%08x, 0x%08x):0x%016llx:: " _f "\n", __FUNCTION__, PC, (PC - (int)&vm->rom), vm->cycle, ##args);
