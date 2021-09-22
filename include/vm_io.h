static uint32_t io_read(vm_p vm, uint32_t pat, uint8_t size)
{
	uint32_t dout = 0;

	for(int i = 0; i < size; i++)
	{
		uint32_t slot = VM_DEVICE_SLOT(pat);
		uint16_t offset = VM_DEVICE_OFFSET(pat);

		dout <<= 8;

		switch(slot)
		{
			case	VM_NVRAM_SLOT:
				dout |= vm->nvram[offset & VM_NVRAM_MASK];
				break;
		}

		pat++;
	}
	return(dout);
}

static void io_write(vm_p vm, uint32_t pat, uint32_t data, uint8_t size)
{
	for(int i = 0; i < size; i++)
	{
		uint32_t slot = VM_DEVICE_SLOT(pat);
		uint16_t offset = VM_DEVICE_OFFSET(pat);
		
		uint8_t dout = (data >> (i << 3)) & 0xff;
		
		switch(slot)
		{
			case	VM_NVRAM_SLOT:
				vm->nvram[offset & VM_NVRAM_MASK] = dout;
				break;
		}
		
		pat++;
	}
}
