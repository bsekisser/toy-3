static void vm_step_3_io_r(_PASS_VM, _PASS_INST)
{
	uint32_t data;
	
	data = io_read(vm, EA, MA.size);

	switch(MA.size)
	{
		case sizeof(uint8_t):
			vR(D) = MA.is_signed ? (int8_t)data : (uint8_t)data;
			break;
		case sizeof(uint16_t):
			vR(D) = MA.is_signed ? (int16_t)data : (uint16_t)data;
			break;
		default:
			vR(D) = data;
	}

	vm->cycle++;
}

static void vm_step_3_io_w(_PASS_VM, _PASS_INST)
{
	io_write(vm, EA, vR(A), MA.size);

	vm->cycle++;
}
	
static void vm_step_3_mem_r(_PASS_VM, _PASS_INST)
{
	switch(MA.size)
	{
		case sizeof(uint8_t):
			vR(D) = MA.is_signed ? *(int8_t*)EA : *(uint8_t*)EA;
			break;
		case sizeof(uint16_t):
			vR(D) = MA.is_signed ? *(int16_t*)EA : *(uint16_t*)EA;
			break;
		default:
			vR(D) = *(uint32_t*)EA;
			break;
	}

	vm->cycle++;
}

static void vm_step_3_mem_w(_PASS_VM, _PASS_INST)
{
	switch(MA.size)
	{
		case sizeof(uint8_t):
			*(uint8_t*)EA = vR(A);
			break;
		case sizeof(uint16_t):
			*(uint16_t*)EA = vR(A);
			break;
		case sizeof(uint32_t):
			*(uint32_t*)EA = vR(A);
			break;
	}

	vm->cycle++;
}

static void vm_step_3_nop(_PASS_VM, _PASS_INST)
{
}
