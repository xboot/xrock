#include <rock.h>

static uint64_t file_save(const char * filename, void * buf, uint64_t len)
{
	FILE * out = fopen(filename, "wb");
	int r;
	if(!out)
	{
		perror("Failed to open output file");
		exit(-1);
	}
	r = fwrite(buf, len, 1, out);
	fclose(out);
	return r;
}

static void * file_load(const char * filename, uint64_t * len)
{
	uint64_t offset = 0, bufsize = 8192;
	char * buf = malloc(bufsize);
	FILE * in;
	if(strcmp(filename, "-") == 0)
		in = stdin;
	else
		in = fopen(filename, "rb");
	if(!in)
	{
		perror("Failed to open input file");
		exit(-1);
	}
	while(1)
	{
		uint64_t len = bufsize - offset;
		uint64_t n = fread(buf + offset, 1, len, in);
		offset += n;
		if(n < len)
			break;
		bufsize *= 2;
		buf = realloc(buf, bufsize);
		if(!buf)
		{
			perror("Failed to resize load_file() buffer");
			exit(-1);
		}
	}
	if(len)
		*len = offset;
	if(in != stdin)
		fclose(in);
	return buf;
}

static void hexdump(uint32_t addr, void * buf, size_t len)
{
	unsigned char * p = buf;
	size_t i, j;

	for(j = 0; j < len; j += 16)
	{
		printf("%08x: ", (uint32_t)(addr + j));
		for(i = 0; i < 16; i++)
		{
			if(j + i < len)
				printf("%02x ", p[j + i]);
			else
				printf("   ");
		}
		putchar(' ');
		for(i = 0; i < 16; i++)
		{
			if(j + i >= len)
				putchar(' ');
			else
				putchar(isprint(p[j + i]) ? p[j + i] : '.');
		}
		printf("\r\n");
	}
}

static void usage(void)
{
	printf("xrock(v1.0.0) - https://github.com/xboot/xrock\r\n");
	printf("usage:\r\n");
	printf("    xrock maskrom <ddr> <usbplug>        - Initial chip using ddr and usbplug in maskrom mode\r\n");
	printf("    xrock version                        - Show chip version\r\n");
	printf("    xrock reset [maskrom]                - Reset device to normal or maskrom mode\n");
	printf("    xrock hexdump <address> <length>     - Dumps memory region in hex\r\n");
	printf("    xrock dump <address> <length>        - Binary memory dump to stdout\r\n");
	printf("    xrock read <address> <length> <file> - Read memory to file\r\n");
	printf("    xrock write <address> <file>         - Write file to memory\r\n");
	printf("    xrock exec <address>                 - Call function address\r\n");
}

int main(int argc, char * argv[])
{
	struct xrock_ctx_t ctx;

	if(argc < 2)
	{
		usage();
		return -1;
	}
	libusb_init(NULL);
	if(!xrock_init(&ctx))
	{
		printf("ERROR: Can't found any rockchip chips\r\n");
		if(ctx.hdl)
			libusb_close(ctx.hdl);
		libusb_exit(NULL);
		return -1;
	}
	if(!strcmp(argv[1], "maskrom"))
	{
		argc -= 2;
		argv += 2;
		if(argc == 2)
		{
			rock_maskrom_init_ddr(&ctx, argv[0]);
			rock_maskrom_init_usbplug(&ctx, argv[1]);
		}
		else
			usage();
	}
	else if(!strcmp(argv[1], "version"))
	{
		uint8_t buf[16];
		if(rock_version(&ctx, buf))
			printf("Chip version: 0x%02x%02x%02x%02x 0x%02x%02x%02x%02x 0x%02x%02x%02x%02x 0x%02x%02x%02x%02x\r\n",
				buf[ 3], buf[ 2], buf[ 1], buf[ 0],
				buf[ 7], buf[ 6], buf[ 5], buf[ 4],
				buf[11], buf[10], buf[ 9], buf[ 8],
				buf[15], buf[14], buf[13], buf[12]);
		else
			printf("Failed to get chip version\r\n");
	}
	else if(!strcmp(argv[1], "reset"))
	{
		if(argc > 2)
		{
			if(!strcmp(argv[2], "maskrom"))
				rock_reset(&ctx, 1);
			else
				usage();
		}
		else
			rock_reset(&ctx, 0);
	}
	else if(!strcmp(argv[1], "hexdump"))
	{
		argc -= 2;
		argv += 2;
		if(argc == 2)
		{
			uint32_t addr = strtoul(argv[0], NULL, 0);
			size_t len = strtoul(argv[1], NULL, 0);
			char * buf = malloc(len);
			if(buf)
			{
				rock_read(&ctx, addr, buf, len);
				hexdump(addr, buf, len);
				free(buf);
			}
		}
		else
			usage();
	}
	else if(!strcmp(argv[1], "dump"))
	{
		argc -= 2;
		argv += 2;
		if(argc == 2)
		{
			uint32_t addr = strtoul(argv[0], NULL, 0);
			size_t len = strtoul(argv[1], NULL, 0);
			char * buf = malloc(len);
			if(buf)
			{
				rock_read(&ctx, addr, buf, len);
				fwrite(buf, len, 1, stdout);
				free(buf);
			}
		}
		else
			usage();
	}
	else if(!strcmp(argv[1], "read"))
	{
		argc -= 2;
		argv += 2;
		if(argc == 3)
		{
			uint32_t addr = strtoul(argv[0], NULL, 0);
			size_t len = strtoul(argv[1], NULL, 0);
			char * buf = malloc(len);
			if(buf)
			{
				rock_read_progress(&ctx, addr, buf, len);
				file_save(argv[2], buf, len);
				free(buf);
			}
		}
		else
			usage();
	}
	else if(!strcmp(argv[1], "write"))
	{
		argc -= 2;
		argv += 2;
		if(argc == 2)
		{
			uint32_t addr = strtoul(argv[0], NULL, 0);
			size_t len;
			void * buf = file_load(argv[1], &len);
			if(buf)
			{
				rock_write_progress(&ctx, addr, buf, len);
				free(buf);
			}
		}
		else
			usage();
	}
	else if(!strcmp(argv[1], "exec"))
	{
		argc -= 2;
		argv += 2;
		if(argc == 1)
		{
			uint32_t addr = strtoul(argv[0], NULL, 0);
			rock_exec(&ctx, addr);
		}
		else
			usage();
	}
	else
	{
		usage();
	}
	if(ctx.hdl)
		libusb_close(ctx.hdl);
	libusb_exit(NULL);

	return 0;
}
