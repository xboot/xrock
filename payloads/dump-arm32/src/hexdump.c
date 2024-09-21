extern void sys_uart_putc(char c);

static inline unsigned char tohex(unsigned char v)
{
	return ((v < 10) ? (v + '0') : (v - 10 + 'a'));
}

void hexdump(unsigned long base, void * buf, int len)
{
	unsigned long o = (unsigned long)base;
	unsigned char * p = (unsigned char *)buf;
	for(int n = 0; n < len; n += 16, o += 16)
	{
		sys_uart_putc(tohex((o >> 28) & 0xf));
		sys_uart_putc(tohex((o >> 24) & 0xf));
		sys_uart_putc(tohex((o >> 20) & 0xf));
		sys_uart_putc(tohex((o >> 16) & 0xf));
		sys_uart_putc(tohex((o >> 12) & 0xf));
		sys_uart_putc(tohex((o >>  8) & 0xf));
		sys_uart_putc(tohex((o >>  4) & 0xf));
		sys_uart_putc(tohex((o >>  0) & 0xf));
		sys_uart_putc(':');
		sys_uart_putc(' ');
		for(int i = 0; i < 16; i++)
		{
			if(n + i < len)
			{
				unsigned char c = p[n + i];
				sys_uart_putc(tohex((c >> 4) & 0xf));
				sys_uart_putc(tohex((c >> 0) & 0xf));
				sys_uart_putc(' ');
			}
			else
			{
				sys_uart_putc(' ');
				sys_uart_putc(' ');
				sys_uart_putc(' ');
			}
		}
		sys_uart_putc('|');
		for(int i = 0; i < 16; i++)
		{
			if(n + i >= len)
				sys_uart_putc(' ');
			else
			{
				unsigned char c = p[n + i];
				if(((unsigned int)c - 0x20) < 0x5f)
					sys_uart_putc(c);
				else
					sys_uart_putc('.');
			}
		}
		sys_uart_putc('\r');
		sys_uart_putc('\n');
	}
}
