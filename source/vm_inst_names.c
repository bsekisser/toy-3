#include "vm_inst_esac_list.h"

#undef INST_ESAC
#define INST_ESAC(_esac, _fn, _action) \
	#_esac,

char* _inst_esac_name_list[128] = {
	INST_ESAC_LIST
};

#undef INST_ESAC
