#include <stdint.h>

#include "bitfield.h"

#include "vm.h"

#include "vm_inst_esac_list.h"
#include "vm_inst_esac_enum.h"


#undef INST_ESAC
#define INST_ESAC(_esac, _type, _decode, _action, _mem, _wb) \
	[INST_ENUM(_esac)] = #_esac,

char* _inst_esac_name_list[256] = {
	INST_ESAC_LIST
};

#undef INST_ESAC
