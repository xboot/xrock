#include <rock.h>

static struct chip_t chips[] = {
	{ 0x281a, "RK2818" },
	{ 0x290a, "RK2918" },
	{ 0x292a, "RK2928" },
	{ 0x292c, "RK3026" },
	{ 0x300a, "RK3066" },
	{ 0x300b, "RK3168" },
	{ 0x301a, "RK3036" },
	{ 0x310a, "RK3066B" },
	{ 0x310b, "RK3188" },
	{ 0x310c, "RK312X" },
	{ 0x320a, "RK3288" },
	{ 0x320b, "RK322X" },
	{ 0x320c, "RK3328" },
	{ 0x330a, "RK3368" },
	{ 0x330c, "RK3399" },
};

int xrock_init(struct xrock_ctx_t * ctx)
{
	if(ctx)
	{
		struct libusb_device_descriptor desc;
		libusb_device_handle * hdl;
		int i;

		ctx->hdl = NULL;
		ctx->chip = NULL;
		for(i = 0; i < ARRAY_SIZE(chips); i++)
		{
			hdl = libusb_open_device_with_vid_pid(NULL, 0x2207, chips[i].pid);
			if(hdl)
			{
				ctx->hdl = hdl;
				ctx->chip = &chips[i];
				break;
			}
		}
		if(ctx->hdl && ctx->chip)
		{
		    if(libusb_kernel_driver_active(ctx->hdl, 0) == 1)
		        libusb_detach_kernel_driver(ctx->hdl, 0);
		    if(libusb_claim_interface(ctx->hdl, 0) == 0)
		    {
		    	if(libusb_get_device_descriptor(libusb_get_device(ctx->hdl), &desc) == 0)
		    	{
		    		if((desc.bcdUSB & 0x0001) == 0x0000)
		    			ctx->maskrom = 1;
		    		else
		    			ctx->maskrom = 0;
		    		return 1;
		    	}
		    }
		}
	}
	return 0;
}

void rock_maskrom_init_ddr(struct xrock_ctx_t * ctx, const char * filename)
{
	struct rc4_ctx_t rc4;
	uint8_t key[16] = { 124, 78, 3, 4, 85, 5, 9, 7, 45, 44, 123, 56, 23, 13, 23, 17 };
	uint16_t crc16 = 0xffff;
	uint8_t buf[4096 + 2];
	FILE * f;
	int n;

	rc4_setkey(&rc4, key, sizeof(key));
	f = fopen(filename, "rb");
	if(!f)
	{
		perror("Failed to open ddr file");
		exit(-1);
	}
	while((n = fread(buf, 1, 4096, f)) == 4096)
	{
		rc4_crypt(&rc4, buf, n);
		crc16 = crc16_sum(crc16, buf, n);
		libusb_control_transfer(ctx->hdl, LIBUSB_REQUEST_TYPE_VENDOR, 12, 0, 1137, buf, n, 0);
	}
	fclose(f);
	if(n >= 0)
	{
		rc4_crypt(&rc4, buf, n);
		crc16 = crc16_sum(crc16, buf, n);
		buf[n++] = crc16 >> 8;
		buf[n++] = crc16 & 0xff;
		libusb_control_transfer(ctx->hdl, LIBUSB_REQUEST_TYPE_VENDOR, 12, 0, 1137, buf, n, 0);
	}
}

void rock_maskrom_init_usbplug(struct xrock_ctx_t * ctx, const char * filename)
{
	struct rc4_ctx_t rc4;
	uint8_t key[16] = { 124, 78, 3, 4, 85, 5, 9, 7, 45, 44, 123, 56, 23, 13, 23, 17 };
	uint16_t crc16 = 0xffff;
	uint8_t buf[4096 + 2];
	FILE * f;
	int n;

	rc4_setkey(&rc4, key, sizeof(key));
	f = fopen(filename, "rb");
	if(!f)
	{
		perror("Failed to open usbplug file");
		exit(-1);
	}
	while((n = fread(buf, 1, 4096, f)) == 4096)
	{
		rc4_crypt(&rc4, buf, n);
		crc16 = crc16_sum(crc16, buf, n);
		libusb_control_transfer(ctx->hdl, LIBUSB_REQUEST_TYPE_VENDOR, 12, 0, 1138, buf, n, 0);
	}
	fclose(f);
	if(n >= 0)
	{
		rc4_crypt(&rc4, buf, n);
		crc16 = crc16_sum(crc16, buf, n);
		buf[n++] = crc16 >> 8;
		buf[n++] = crc16 & 0xff;
		libusb_control_transfer(ctx->hdl, LIBUSB_REQUEST_TYPE_VENDOR, 12, 0, 1138, buf, n, 0);
	}
}



struct usb_request_t {
	char magic[8];
	uint32_t length;
	uint32_t unknown1;
	uint16_t request;
	uint32_t length2;
	char pad[10];
} __attribute__((packed));

struct fel_request_t {
	uint32_t request;
	uint32_t address;
	uint32_t length;
	uint32_t pad;
} __attribute__((packed));

static inline void usb_bulk_send(libusb_device_handle * hdl, int ep, const char * buf, size_t len)
{
	size_t max_chunk = 128 * 1024;
	size_t chunk;
	int r, bytes;

	while(len > 0)
	{
		chunk = len < max_chunk ? len : max_chunk;
		r = libusb_bulk_transfer(hdl, ep, (void *)buf, chunk, &bytes, 10000);
		if(r != 0)
		{
			printf("usb bulk send error\r\n");
			exit(-1);
		}
		len -= bytes;
		buf += bytes;
	}
}

static inline void usb_bulk_recv(libusb_device_handle * hdl, int ep, char * buf, size_t len)
{
	int r, bytes;

	while(len > 0)
	{
		r = libusb_bulk_transfer(hdl, ep, (void *)buf, len, &bytes, 10000);
		if(r != 0)
		{
			printf("usb bulk recv error\r\n");
			exit(-1);
		}
		len -= bytes;
		buf += bytes;
	}
}

static inline void send_usb_request(struct xrock_ctx_t * ctx, int type, size_t length)
{
	struct usb_request_t req = {
		.magic = "AWUC",
		.request = cpu_to_le16(type),
		.length = cpu_to_le32(length),
		.unknown1 = cpu_to_le32(0x0c000000)
	};
	req.length2 = req.length;
	usb_bulk_send(ctx->hdl, ctx->epout, (const char *)&req, sizeof(struct usb_request_t));
}

static inline void read_usb_response(struct xrock_ctx_t * ctx)
{
	char buf[13];

	usb_bulk_recv(ctx->hdl, ctx->epin, (char *)buf, sizeof(buf));
	assert(strcmp(buf, "AWUS") == 0);
}

static inline void usb_write(struct xrock_ctx_t * ctx, const void * buf, size_t len)
{
	send_usb_request(ctx, 0x12, len);
	usb_bulk_send(ctx->hdl, ctx->epout, (const char *)buf, len);
	read_usb_response(ctx);
}

static inline void usb_read(struct xrock_ctx_t * ctx, const void * data, size_t len)
{
	send_usb_request(ctx, 0x11, len);
	usb_bulk_send(ctx->hdl, ctx->epin, (const char *)data, len);
	read_usb_response(ctx);
}

static inline void send_fel_request(struct xrock_ctx_t * ctx, int type, uint32_t addr, uint32_t length)
{
	struct fel_request_t req = {
		.request = cpu_to_le32(type),
		.address = cpu_to_le32(addr),
		.length = cpu_to_le32(length)
	};
	usb_write(ctx, &req, sizeof(struct fel_request_t));
}

static inline void read_fel_status(struct xrock_ctx_t * ctx)
{
	char buf[8];
	usb_read(ctx, buf, sizeof(buf));
}

static inline void fel_version(struct xrock_ctx_t * ctx)
{
	int i;

	send_fel_request(ctx, 0x001, 0, 0);
	usb_read(ctx, &ctx->version, sizeof(ctx->version));
	read_fel_status(ctx);
	ctx->version.id = le32_to_cpu(ctx->version.id);
	ctx->version.firmware = le32_to_cpu(ctx->version.firmware);
	ctx->version.protocol = le16_to_cpu(ctx->version.protocol);
	ctx->version.scratchpad = le32_to_cpu(ctx->version.scratchpad);
	ctx->chip = NULL;
	for(i = 0; i < ARRAY_SIZE(chips); i++)
	{
/*		if(chips[i]->id == ctx->version.id)
		{
			ctx->chip = chips[i];
			break;
		}*/
	}
}

int fel_init(struct xrock_ctx_t * ctx)
{
	if(ctx && ctx->hdl)
	{
		struct libusb_config_descriptor * config;
		int if_idx, set_idx, ep_idx;
		const struct libusb_interface * iface;
		const struct libusb_interface_descriptor * setting;
		const struct libusb_endpoint_descriptor * ep;
		if(libusb_claim_interface(ctx->hdl, 0) == 0)
		{
			if(libusb_get_active_config_descriptor(libusb_get_device(ctx->hdl), &config) == 0)
			{
				for(if_idx = 0; if_idx < config->bNumInterfaces; if_idx++)
				{
					iface = config->interface + if_idx;
					for(set_idx = 0; set_idx < iface->num_altsetting; set_idx++)
					{
						setting = iface->altsetting + set_idx;
						for(ep_idx = 0; ep_idx < setting->bNumEndpoints; ep_idx++)
						{
							ep = setting->endpoint + ep_idx;
							if((ep->bmAttributes & LIBUSB_TRANSFER_TYPE_MASK) != LIBUSB_TRANSFER_TYPE_BULK)
								continue;
							if((ep->bEndpointAddress & LIBUSB_ENDPOINT_DIR_MASK) == LIBUSB_ENDPOINT_IN)
								ctx->epin = ep->bEndpointAddress;
							else
								ctx->epout = ep->bEndpointAddress;
						}
					}
				}
				libusb_free_config_descriptor(config);
				fel_version(ctx);
				return 1;
			}
		}
	}
	return 0;
}

void fel_exec(struct xrock_ctx_t * ctx, uint32_t addr)
{
	send_fel_request(ctx, 0x102, addr, 0);
	read_fel_status(ctx);
}

static inline void fel_read_raw(struct xrock_ctx_t * ctx, uint32_t addr, void * buf, size_t len)
{
	send_fel_request(ctx, 0x103, addr, len);
	usb_read(ctx, buf, len);
	read_fel_status(ctx);
}

static inline void fel_write_raw(struct xrock_ctx_t * ctx, uint32_t addr, void * buf, size_t len)
{
	send_fel_request(ctx, 0x101, addr, len);
	usb_write(ctx, buf, len);
	read_fel_status(ctx);
}

uint32_t fel_read32(struct xrock_ctx_t * ctx, uint32_t addr)
{
	uint32_t val;
	fel_read_raw(ctx, addr, &val, sizeof(uint32_t));
	return val;
}

void fel_write32(struct xrock_ctx_t * ctx, uint32_t addr, uint32_t val)
{
	fel_write_raw(ctx, addr, &val, sizeof(uint32_t));
}

void fel_read(struct xrock_ctx_t * ctx, uint32_t addr, void * buf, size_t len)
{
	size_t n;

	while(len > 0)
	{
		n = len > 65536 ? 65536 : len;
		fel_read_raw(ctx, addr, buf, n);
		addr += n;
		buf += n;
		len -= n;
	}
}

void fel_write(struct xrock_ctx_t * ctx, uint32_t addr, void * buf, size_t len)
{
	size_t n;

	while(len > 0)
	{
		n = len > 65536 ? 65536 : len;
		fel_write_raw(ctx, addr, buf, n);
		addr += n;
		buf += n;
		len -= n;
	}
}

void fel_read_progress(struct xrock_ctx_t * ctx, uint32_t addr, void * buf, size_t len)
{
	struct progress_t p;
	size_t n;

	progress_start(&p, len);
	while(len > 0)
	{
		n = len > 65536 ? 65536 : len;
		fel_read_raw(ctx, addr, buf, n);
		addr += n;
		buf += n;
		len -= n;
		progress_update(&p, n);
	}
	progress_stop(&p);
}

void fel_write_progress(struct xrock_ctx_t * ctx, uint32_t addr, void * buf, size_t len)
{
	struct progress_t p;
	size_t n;

	progress_start(&p, len);
	while(len > 0)
	{
		n = len > 65536 ? 65536 : len;
		fel_write_raw(ctx, addr, buf, n);
		addr += n;
		buf += n;
		len -= n;
		progress_update(&p, n);
	}
	progress_stop(&p);
}
