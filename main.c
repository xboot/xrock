#include <rock.h>

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
	printf("xrock(v1.0.7) - https://github.com/xboot/xrock\r\n");

	printf("usage:\r\n");
	printf("    xrock maskrom <ddr> <usbplug> [--rc4-off] - Initial chip using ddr and usbplug in maskrom mode\r\n");
	printf("    xrock download <loader>                   - Initial chip using loader in maskrom mode\r\n");
	printf("    xrock ready                               - Show chip ready or not\r\n");
	printf("    xrock version                             - Show chip version\r\n");
	printf("    xrock capability                          - Show capability information\r\n");
	printf("    xrock reset [maskrom]                     - Reset chip to normal or maskrom mode\n");
	printf("    xrock hexdump <address> <length>          - Dumps memory region in hex\r\n");
	printf("    xrock dump <address> <length>             - Binary memory dump to stdout\r\n");
	printf("    xrock read <address> <length> <file>      - Read memory to file\r\n");
	printf("    xrock write <address> <file>              - Write file to memory\r\n");
	printf("    xrock exec <address> [dtb]                - Call function address\r\n");
	printf("    xrock flash                               - Detect flash and show information\r\n");
	printf("    xrock flash erase <sector> <count>        - Erase flash sector\r\n");
	printf("    xrock flash read <sector> <count> <file>  - Read flash sector to file\r\n");
	printf("    xrock flash write <sector> <file>         - Write file to flash sector\r\n");

	printf("extra:\r\n");
	printf("    xrock extra maskrom --rc4 <on|off> [--sram <file> --delay <ms>] [--dram <file> --delay <ms>] [...]\r\n");
}

int main(int argc, char * argv[])
{
	struct xrock_ctx_t ctx;

	if(argc < 2)
	{
		usage();
		return 0;
	}
	for(int i = 1; i < argc; i++)
	{
		if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
		{
			usage();
			return 0;
		}
	}

	libusb_init(&ctx.context);
	if(!xrock_init(&ctx))
	{
		printf("ERROR: Can't found any supported rockchip chips\r\n");
		if(ctx.hdl)
			libusb_close(ctx.hdl);
		libusb_exit(ctx.context);
		return -1;
	}
	if(!strcmp(argv[1], "maskrom"))
	{
		argc -= 2;
		argv += 2;
		if(argc >= 2)
		{
			if(ctx.maskrom)
			{
				int rc4 = 1;
				if((argc == 3) && !strcmp(argv[2], "--rc4-off"))
					rc4 = 0;
				rock_maskrom_upload_file(&ctx, 0x471, argv[0], rc4);
				usleep(10 * 1000);
				rock_maskrom_upload_file(&ctx, 0x472, argv[1], rc4);
				usleep(10 * 1000);
			}
			else
				printf("ERROR: The chip '%s' does not in maskrom mode\r\n", ctx.chip->name);
		}
		else
			usage();
	}
	else if(!strcmp(argv[1], "download"))
	{
		argc -= 2;
		argv += 2;
		if(argc == 1)
		{
			if(ctx.maskrom)
			{
				struct rkloader_ctx_t * bctx = rkloader_ctx_alloc(argv[0]);
				if(bctx)
				{
					for(int i = 0; i < bctx->nentry; i++)
					{
						struct rkloader_entry_t * e = bctx->entry[i];
						char str[256];
						if(e->type == RKLOADER_ENTRY_471)
						{
							void * buf = (char *)bctx->buffer + rkloader_read_le32(&e->data_offset);
							uint64_t len = rkloader_read_le32(&e->data_size);
							uint32_t delay = rkloader_read_le32(&e->data_delay);

							printf("Downloading '%s'\r\n", loader_wide2str(str, (uint8_t *)&e->name[0], sizeof(e->name)));
							rock_maskrom_upload_memory(&ctx, 0x471, buf, len, bctx->header->rc4_flag ? 0 : 1);
							usleep(delay * 1000);
						}
						else if(e->type == RKLOADER_ENTRY_472)
						{
							void * buf = (char *)bctx->buffer + rkloader_read_le32(&e->data_offset);
							uint64_t len = rkloader_read_le32(&e->data_size);
							uint32_t delay = rkloader_read_le32(&e->data_delay);

							printf("Downloading '%s'\r\n", loader_wide2str(str, (uint8_t *)&e->name[0], sizeof(e->name)));
							rock_maskrom_upload_memory(&ctx, 0x472, buf, len, bctx->header->rc4_flag ? 0 : 1);
							usleep(delay * 1000);
						}
					}
					rkloader_ctx_free(bctx);
				}
				else
					printf("ERROR: Not a valid loader '%s'\r\n", argv[0]);
			}
			else
				printf("ERROR: The chip '%s' does not in maskrom mode\r\n", ctx.chip->name);
		}
		else
			usage();
	}
	else if(!strcmp(argv[1], "ready"))
	{
		argc -= 2;
		argv += 2;
		if(argc == 0)
		{
			if(rock_ready(&ctx))
				printf("The chip is ready\r\n");
			else
				printf("Failed to show chip ready status\r\n");
		}
		else
			usage();
	}
	else if(!strcmp(argv[1], "version"))
	{
		argc -= 2;
		argv += 2;
		if(argc == 0)
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
		else
			usage();
	}
	else if(!strcmp(argv[1], "capability"))
	{
		argc -= 2;
		argv += 2;
		if(argc == 0)
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
				printf("    Switch Storage: %s\r\n", (buf[1] & (1 << 1)) ? "enabled" : "disabled");
				printf("    LBA Parity: %s\r\n", (buf[1] & (1 << 2)) ? "enabled" : "disabled");
				printf("    Read OTP Chip: %s\r\n", (buf[1] & (1 << 3)) ? "enabled" : "disabled");
				printf("    Switch USB3: %s\r\n", (buf[1] & (1 << 4)) ? "enabled" : "disabled");
			}
			else
				printf("Failed to show capability information\r\n");
		}
		else
			usage();
	}
	else if(!strcmp(argv[1], "reset"))
	{
		argc -= 2;
		argv += 2;
		if(argc > 0)
		{
			if(!strcmp(argv[0], "maskrom"))
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
			uint64_t len;
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
		if(argc >= 1)
		{
			uint32_t addr = strtoul(argv[0], NULL, 0);
			uint32_t dtb = (argc >= 2) ? strtoul(argv[1], NULL, 0) : 0;
			rock_exec(&ctx, addr, dtb);
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
				printf("Flash info:\r\n");
				printf("    Manufacturer: %s (%d)\r\n", (info.manufacturer_id < ARRAY_SIZE(manufacturer))
								? manufacturer[info.manufacturer_id] : "Unknown", info.manufacturer_id);
				printf("    Capacity: %dMB\r\n", info.sector_total >> 11);
				printf("    Sector size: %d\r\n", 512);
				printf("    Sector count: %d\r\n", info.sector_total);
				printf("    Block size: %dKB\r\n", info.block_size >> 1);
				printf("    Page size: %dKB\r\n", info.page_size >> 1);
				printf("    ECC bits: %d\r\n", info.ecc_bits);
				printf("    Access time: %d\r\n", info.access_time);
				printf("    Flash CS: %s%s%s%s\r\n",
								info.chip_select & 1 ? "<0>" : "",
								info.chip_select & 2 ? "<1>" : "",
								info.chip_select & 4 ? "<2>" : "",
								info.chip_select & 8 ? "<3>" : "");
				printf("    Flash ID: %02x %02x %02x %02x %02x\r\n",
								info.id[0], info.id[1],	info.id[2],	info.id[3],	info.id[4]);
			}
			else
				printf("Failed to detect flash\r\n");
		}
		else
		{
			if(!strcmp(argv[0], "erase") && (argc == 3))
			{
				argc -= 1;
				argv += 1;
				struct flash_info_t info;
				uint32_t sec = strtoul(argv[0], NULL, 0);
				uint32_t cnt = strtoul(argv[1], NULL, 0);
				if(rock_flash_detect(&ctx, &info))
				{
					if(sec < info.sector_total)
					{
						if(cnt <= 0)
							cnt = info.sector_total - sec;
						else if(cnt > info.sector_total - sec)
							cnt = info.sector_total - sec;
						if(!rock_flash_erase_lba_progress(&ctx, sec, cnt))
							printf("Failed to erase flash\r\n");
					}
					else
						printf("The start sector is out of range\r\n");
				}
				else
					printf("Failed to detect flash\r\n");
			}
			else if(!strcmp(argv[0], "read") && (argc == 4))
			{
				argc -= 1;
				argv += 1;
				struct flash_info_t info;
				uint32_t sec = strtoul(argv[0], NULL, 0);
				uint32_t cnt = strtoul(argv[1], NULL, 0);
				if(rock_flash_detect(&ctx, &info))
				{
					if(sec < info.sector_total)
					{
						if(cnt <= 0)
							cnt = info.sector_total - sec;
						else if(cnt > info.sector_total - sec)
							cnt = info.sector_total - sec;
						char * buf = malloc((uint64_t)cnt << 9);
						if(buf)
						{
							if(rock_flash_read_lba_progress(&ctx, sec, cnt, buf))
								file_save(argv[2], buf, (uint64_t)cnt << 9);
							else
								printf("Failed to read flash\r\n");
							free(buf);
						}
					}
					else
						printf("The start sector is out of range\r\n");
				}
				else
					printf("Failed to detect flash\r\n");
			}
			else if(!strcmp(argv[0], "write") && (argc == 3))
			{
				argc -= 1;
				argv += 1;
				struct flash_info_t info;
				uint32_t sec = strtoul(argv[0], NULL, 0);
				uint32_t cnt;
				void * buf;
				uint64_t len;
				if(rock_flash_detect(&ctx, &info))
				{
					if(sec < info.sector_total)
					{
						buf = file_load(argv[1], &len);
						if(buf)
						{
							if(len % 512 != 0)
							{
								cnt = (len >> 9) + 1;
								buf = realloc(buf, (uint64_t)cnt << 9);
							}
							else
								cnt = (len >> 9);
							if(cnt <= 0)
								cnt = info.sector_total - sec;
							else if(cnt > info.sector_total - sec)
								cnt = info.sector_total - sec;
							if(!rock_flash_write_lba_progress(&ctx, sec, cnt, buf))
								printf("Failed to write flash\\r\n");
							free(buf);
						}
					}
					else
						printf("The start sector is out of range\r\n");
				}
				else
					printf("Failed to detect flash\r\n");
			}
			else
				usage();
		}
	}
	else if(!strcmp(argv[1], "extra"))
	{
		argc -= 2;
		argv += 2;
		if(!strcmp(argv[0], "maskrom"))
		{
			argc -= 1;
			argv += 1;
			if(argc >= 2)
			{
				if(ctx.maskrom)
				{
					int rc4 = 0;
					for(int i = 0; i < argc; i++)
					{
						if(!strcmp(argv[i], "--rc4") && (argc > i + 1))
						{
							if(!strcmp(argv[i + 1], "on"))
								rc4 = 1;
							else if(!strcmp(argv[i + 1], "off"))
								rc4 = 0;
							i++;
						}
						else if(!strcmp(argv[i], "--sram") && (argc > i + 1))
						{
							rock_maskrom_upload_file(&ctx, 0x471, argv[i + 1], rc4);
							i++;
						}
						else if(!strcmp(argv[i], "--dram") && (argc > i + 1))
						{
							rock_maskrom_upload_file(&ctx, 0x472, argv[i + 1], rc4);
							i++;
						}
						else if(!strcmp(argv[i], "--delay") && (argc > i + 1))
						{
							uint32_t delay = strtoul(argv[i + 1], NULL, 0) * 1000;
							usleep(delay);
							i++;
						}
						else if(*argv[i] == '-')
						{
							usage();
						}
						else if(*argv[i] != '-' && strcmp(argv[i], "-") != 0)
						{
							usage();
						}
					}
				}
				else
					printf("ERROR: The chip '%s' does not in maskrom mode\r\n", ctx.chip->name);
			}
			else
				usage();
		}
		else
			usage();
	}
	else
		usage();
	if(ctx.hdl)
		libusb_close(ctx.hdl);
	libusb_exit(ctx.context);

	return 0;
}
