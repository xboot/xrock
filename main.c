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

static const char * manufacturer[] = {
	"Samsung",
	"Toshiba",
	"Hynix",
	"Infineon",
	"Micron",
	"Renesas",
	"ST",
	"Intel",
	"SanDisk",
};

static void usage(void)
{
	printf("xrock(v1.0.1) - https://github.com/xboot/xrock\r\n");
	printf("usage:\r\n");
	printf("    xrock maskrom <ddr> <usbplug>            - Initial chip using ddr and usbplug in maskrom mode\r\n");
	printf("    xrock version                            - Show chip version\r\n");
	printf("    xrock capability                         - Show capability information\r\n");
	printf("    xrock reset [maskrom]                    - Reset chip to normal or maskrom mode\n");
	printf("    xrock hexdump <address> <length>         - Dumps memory region in hex\r\n");
	printf("    xrock dump <address> <length>            - Binary memory dump to stdout\r\n");
	printf("    xrock read <address> <length> <file>     - Read memory to file\r\n");
	printf("    xrock write <address> <file>             - Write file to memory\r\n");
	printf("    xrock exec <address>                     - Call function address\r\n");
	printf("    xrock flash                              - Detect flash and show information\r\n");
	printf("    xrock flash erase <sector> <count>       - Erase flash sector\r\n");
	printf("    xrock flash read <sector> <count> <file> - Read flash sector to file\r\n");
	printf("    xrock flash write <sector> <file>        - Write file to flash sector\r\n");
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
		printf("ERROR: Can't found any supported rockchip chips\r\n");
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
			if(ctx.maskrom)
			{
				rock_maskrom_init_ddr(&ctx, argv[0]);
				rock_maskrom_init_usbplug(&ctx, argv[1]);
			}
			else
				printf("ERROR: The chip '%s' does not in maskrom mode\r\n", ctx.chip->name);
		}
		else
			usage();
	}
	else if(!strcmp(argv[1], "version"))
	{
		uint8_t buf[16];
		if(rock_version(&ctx, buf))
			printf("%s(%c%c%c%c): 0x%02x%02x%02x%02x 0x%02x%02x%02x%02x 0x%02x%02x%02x%02x 0x%02x%02x%02x%02x\r\n", ctx.chip->name,
				buf[ 3], buf[ 2], buf[ 1], buf[ 0],
				buf[ 3], buf[ 2], buf[ 1], buf[ 0],
				buf[ 7], buf[ 6], buf[ 5], buf[ 4],
				buf[11], buf[10], buf[ 9], buf[ 8],
				buf[15], buf[14], buf[13], buf[12]);
		else
			printf("Failed to get chip version\r\n");
	}
	else if(!strcmp(argv[1], "capability"))
	{
		uint8_t buf[8];
		if(rock_capability(&ctx, buf))
		{
			printf("Capability: %02x %02x %02x %02x %02x %02x %02x %02x\r\n",
				buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
			printf("    Direct LBA: %s\r\n", (buf[0] & (1 << 0)) ? "enabled" : "disabled");
			printf("    Vendor Storage: %s\r\n", (buf[0] & (1 << 1)) ? "enabled" : "disabled");
			printf("    First 4M Access: %s\r\n", (buf[0] & (1 << 2)) ? "enabled" : "disabled");
			printf("    Read LBA: %s\r\n", (buf[0] & (1 << 3)) ? "enabled" : "disabled");
			printf("    Read Com Log: %s\r\n", (buf[0] & (1 << 5)) ? "enabled" : "disabled");
			printf("    Read IDB Config: %s\r\n", (buf[0] & (1 << 6)) ? "enabled" : "disabled");
			printf("    Read Secure Mode: %s\r\n", (buf[0] & (1 << 7)) ? "enabled" : "disabled");
			printf("    New IDB: %s\r\n", (buf[1] & (1 << 0)) ? "enabled" : "disabled");
		}
		else
			printf("Failed to show capability information\r\n");
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
	else if(!strcmp(argv[1], "flash"))
	{
		argc -= 2;
		argv += 2;
		if(argc == 0)
		{
			struct flash_info_t info;
			if(rock_flash_detect(&ctx, &info))
			{
				printf("Flash Info:\r\n");
				printf("    Manufacturer: %s (%d)\r\n", (info.manufacturer_id < ARRAY_SIZE(manufacturer))
								? manufacturer[info.manufacturer_id] : "Unknown", info.manufacturer_id);
				printf("    Flash Size: %dMB\r\n", info.flash_size >> 11);
				printf("    Sectors: %d\r\n", info.flash_size);
				printf("    Block Size: %dKB\r\n", info.block_size >> 1);
				printf("    Page Size: %dKB\r\n", info.page_size >> 1);
				printf("    ECC Bits: %d\r\n", info.ecc_bits);
				printf("    Access Time: %d\r\n", info.access_time);
				printf("    Flash CS: %s%s%s%s\r\n",
								info.chip_select & 1 ? "<0>" : "",
								info.chip_select & 2 ? "<1>" : "",
								info.chip_select & 4 ? "<2>" : "",
								info.chip_select & 8 ? "<3>" : "");
				printf("    Flash ID: %02x %02x %02x %02x %02x\r\n",
								info.id[0], info.id[1],	info.id[2],	info.id[3],	info.id[4]);
			}
			else
				printf("Failed to show flash information\r\n");
		}
		else
		{
			if(!strcmp(argv[0], "erase") && (argc == 3))
			{
				argc -= 1;
				argv += 1;
				uint32_t sec = strtoul(argv[0], NULL, 0);
				uint32_t cnt = strtoul(argv[1], NULL, 0);
				if(!rock_flash_erase_lba_progress(&ctx, sec, cnt))
					printf("Failed to erase flash\r\n");
			}
			else if(!strcmp(argv[0], "read") && (argc == 4))
			{
				argc -= 1;
				argv += 1;
				uint32_t sec = strtoul(argv[0], NULL, 0);
				uint32_t cnt = strtoul(argv[1], NULL, 0);
				char * buf = malloc(cnt << 9);
				if(buf)
				{
					if(rock_flash_read_lba_progress(&ctx, sec, cnt, buf))
						file_save(argv[2], buf, cnt << 9);
					else
						printf("Failed to read flash\r\n");
					free(buf);
				}
			}
			else if(!strcmp(argv[0], "write") && (argc == 3))
			{
				argc -= 1;
				argv += 1;
				uint32_t sec = strtoul(argv[0], NULL, 0);
				uint32_t cnt;
				uint64_t len;
				void * buf = file_load(argv[1], &len);
				if(buf)
				{
					if(len % 512 != 0)
					{
						cnt = (len >> 9) + 1;
						buf = realloc(buf, cnt << 9);
					}
					else
						cnt = (len >> 9);
					if(!rock_flash_write_lba_progress(&ctx, sec, cnt, buf))
						printf("Failed to write flash\\r\n");
					free(buf);
				}
			}
			else
				usage();
		}
	}
	else
		usage();
	if(ctx.hdl)
		libusb_close(ctx.hdl);
	libusb_exit(NULL);

	return 0;
}
