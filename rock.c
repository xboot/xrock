#include <rock.h>

static struct chip_t chips[] = {
	{ 0x281a, "RK2818" },
	{ 0x290a, "RK2918" },
	{ 0x292a, "RK2928" },
	{ 0x292c, "RK3026" },
	{ 0x300a, "RK3066" },
	{ 0x300b, "RK3168" },
	{ 0x301a, "RK3036" },
	{ 0x310a, "RK3066" },
	{ 0x310b, "RK3188" },
	{ 0x310c, "RK3128" },
	{ 0x320a, "RK3288" },
	{ 0x320b, "RK3228" },
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
		    		ctx->epout = 2 | LIBUSB_ENDPOINT_OUT;
		    		ctx->epin = 1 | LIBUSB_ENDPOINT_IN;
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
		exit(-1);
	while((n = fread(buf, 1, 4096, f)) == 4096)
	{
		rc4_crypt(&rc4, buf, n);
		crc16 = crc16_sum(crc16, buf, n);
		libusb_control_transfer(ctx->hdl, LIBUSB_REQUEST_TYPE_VENDOR, 12, 0, 0x471, buf, n, 0);
	}
	fclose(f);
	if(n >= 0)
	{
		rc4_crypt(&rc4, buf, n);
		crc16 = crc16_sum(crc16, buf, n);
		buf[n++] = crc16 >> 8;
		buf[n++] = crc16 & 0xff;
		libusb_control_transfer(ctx->hdl, LIBUSB_REQUEST_TYPE_VENDOR, 12, 0, 0x471, buf, n, 0);
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
		exit(-1);
	while((n = fread(buf, 1, 4096, f)) == 4096)
	{
		rc4_crypt(&rc4, buf, n);
		crc16 = crc16_sum(crc16, buf, n);
		libusb_control_transfer(ctx->hdl, LIBUSB_REQUEST_TYPE_VENDOR, 12, 0, 0x472, buf, n, 0);
	}
	fclose(f);
	if(n >= 0)
	{
		rc4_crypt(&rc4, buf, n);
		crc16 = crc16_sum(crc16, buf, n);
		buf[n++] = crc16 >> 8;
		buf[n++] = crc16 & 0xff;
		libusb_control_transfer(ctx->hdl, LIBUSB_REQUEST_TYPE_VENDOR, 12, 0, 0x472, buf, n, 0);
	}
}

enum {
	USB_REQUEST_SIGN		= 0x55534243,	/* "USBC" */
	USB_RESPONSE_SIGN		= 0x55534253,	/* "USBS" */
};

enum {
	USB_DIRECTION_OUT		= 0x00,
	USB_DIRECTION_IN		= 0x80,
};

enum {
	OPCODE_TEST_UNIT_READY	= 0x00,
	OPCODE_READ_FLASH_ID	= 0x01,
	OPCODE_TEST_BAD_BLOCK	= 0x03,
	OPCODE_READ_SECTOR		= 0x04,
	OPCODE_WRITE_SECTOR		= 0x05,
	OPCODE_ERASE_NORMAL		= 0x06,
	OPCODE_ERASE_FORCE		= 0x0b,
	OPCODE_READ_LBA			= 0x14,
	OPCODE_WRITE_LBA		= 0x15,
	OPCODE_ERASE_SYSTEM		= 0x16,
	OPCODE_READ_SDRAM		= 0x17,
	OPCODE_WRITE_SDRAM		= 0x18,
	OPCODE_EXEC_SDRAM		= 0x19,
	OPCODE_READ_FLASH_INFO	= 0x1a,
	OPCODE_READ_CHIP_INFO	= 0x1b,
	OPCODE_SET_RESET_FLAG	= 0x1e,
	OPCODE_WRITE_EFUSE		= 0x1f,
	OPCODE_READ_EFUSE		= 0x20,
	OPCODE_READ_SPI_FLASH	= 0x21,
	OPCODE_WRITE_SPI_FLASH	= 0x22,
	OPCODE_WRITE_NEW_EFUSE	= 0x23,
	OPCODE_READ_NEW_EFUSE	= 0x24,
	OPCODE_ERASE_LBA		= 0x25,
	OPCODE_READ_CAPABILITY	= 0xaa,
	OPCODE_RESET_DEVICE		= 0xff,
};

struct usb_request_t {
	uint32_t signature;		/* Contains 'USBC' */
	uint32_t tag;			/* The random unique id */
	uint32_t dsize;			/* The size of data */
	uint8_t flag;			/* Direction in bit 7 */
	uint8_t lun;			/* LUN (normally 0) */
	uint8_t length;			/* Length ... */
	uint8_t opcode;			/* The command of opcode */
	uint8_t subcode;		/* The command of subcode */
	uint32_t address;		/* Address */
	uint8_t reserved2;
	uint16_t size;
	uint8_t reserved3[7];
} __attribute__((packed));

struct usb_response_t {
	uint32_t signature;		/* Contains 'USBS' */
	uint32_t tag;			/* Same as original command */
	uint32_t residue;		/* Amount not transferred */
	uint8_t status;			/* Response status */
} __attribute__((packed));

static inline void usb_bulk_send(libusb_device_handle * hdl, int ep, void * buf, size_t len)
{
	size_t max_chunk = 128 * 1024;
	size_t chunk;
	int r, bytes;

	while(len > 0)
	{
		chunk = len < max_chunk ? len : max_chunk;
		r = libusb_bulk_transfer(hdl, ep, (void *)buf, chunk, &bytes, 2000);
		if(r != 0)
		{
			printf("usb bulk send error\r\n");
			exit(-1);
		}
		len -= bytes;
		buf += bytes;
	}
}

static inline void usb_bulk_recv(libusb_device_handle * hdl, int ep, void * buf, size_t len)
{
	int r, bytes;

	while(len > 0)
	{
		r = libusb_bulk_transfer(hdl, ep, (void *)buf, len, &bytes, 2000);
		if(r != 0)
		{
			printf("usb bulk recv error\r\n");
			exit(-1);
		}
		len -= bytes;
		buf += bytes;
	}
}

static inline uint32_t make_tag(void)
{
	uint32_t tag = 0;
	int i;

	for(i = 0; i < 4; i++)
		tag = (tag << 8) | (rand() & 0xff);
	return tag;
}

int rock_version(struct xrock_ctx_t * ctx, uint8_t * buf)
{
	struct usb_request_t req;
	struct usb_response_t res;

	memset(&req, 0, sizeof(struct usb_request_t));
	req.signature = cpu_to_be32(USB_REQUEST_SIGN);
	req.tag = cpu_to_be32(make_tag());
	req.dsize = 16;
	req.flag = USB_DIRECTION_IN;
	req.length = 6;
	req.opcode = OPCODE_READ_CHIP_INFO;

	usb_bulk_send(ctx->hdl, ctx->epout, &req, sizeof(struct usb_request_t));
	usb_bulk_recv(ctx->hdl, ctx->epin, buf, 16);
	usb_bulk_recv(ctx->hdl, ctx->epin, &res, sizeof(struct usb_response_t));
	if((be32_to_cpu(res.signature) != USB_RESPONSE_SIGN) || (res.tag != req.tag))
		return 0;
	return 1;
}

int rock_reset(struct xrock_ctx_t * ctx, int maskrom)
{
	struct usb_request_t req;
	struct usb_response_t res;

	memset(&req, 0, sizeof(struct usb_request_t));
	req.signature = cpu_to_be32(USB_REQUEST_SIGN);
	req.tag = cpu_to_be32(make_tag());
	req.flag = USB_DIRECTION_OUT;
	req.length = 6;
	req.opcode = OPCODE_RESET_DEVICE;
	req.subcode = maskrom ? 0x03 : 0x00;

	usb_bulk_send(ctx->hdl, ctx->epout, &req, sizeof(struct usb_request_t));
	usb_bulk_recv(ctx->hdl, ctx->epin, &res, sizeof(struct usb_response_t));
	if((be32_to_cpu(res.signature) != USB_RESPONSE_SIGN) || (res.tag != req.tag))
		return 0;
	return 1;
}

int rock_exec(struct xrock_ctx_t * ctx, uint32_t addr)
{
	struct usb_request_t req;
	struct usb_response_t res;

	memset(&req, 0, sizeof(struct usb_request_t));
	req.signature = cpu_to_be32(USB_REQUEST_SIGN);
	req.tag = cpu_to_be32(make_tag());
	req.flag = USB_DIRECTION_OUT;
	req.length = 10;
	req.opcode = OPCODE_EXEC_SDRAM;
	req.address = cpu_to_be32(addr);

	usb_bulk_send(ctx->hdl, ctx->epout, &req, sizeof(struct usb_request_t));
	usb_bulk_recv(ctx->hdl, ctx->epin, &res, sizeof(struct usb_response_t));
	if((be32_to_cpu(res.signature) != USB_RESPONSE_SIGN) || (res.tag != req.tag))
		return 0;
	return 1;
}

static inline int rock_read_raw(struct xrock_ctx_t * ctx, uint32_t addr, void * buf, size_t len)
{
	struct usb_request_t req;
	struct usb_response_t res;

	memset(&req, 0, sizeof(struct usb_request_t));
	req.signature = cpu_to_be32(USB_REQUEST_SIGN);
	req.tag = cpu_to_be32(make_tag());
	req.flag = USB_DIRECTION_IN;
	req.length = 10;
	req.opcode = OPCODE_READ_SDRAM;
	req.address = cpu_to_be32(addr);
	req.size = cpu_to_be16(len);

	usb_bulk_send(ctx->hdl, ctx->epout, &req, sizeof(struct usb_request_t));
	usb_bulk_recv(ctx->hdl, ctx->epin, buf, len);
	usb_bulk_recv(ctx->hdl, ctx->epin, &res, sizeof(struct usb_response_t));
	if((be32_to_cpu(res.signature) != USB_RESPONSE_SIGN) || (res.tag != req.tag))
		return 0;
	return 1;
}

static inline int rock_write_raw(struct xrock_ctx_t * ctx, uint32_t addr, void * buf, size_t len)
{
	struct usb_request_t req;
	struct usb_response_t res;

	memset(&req, 0, sizeof(struct usb_request_t));
	req.signature = cpu_to_be32(USB_REQUEST_SIGN);
	req.tag = cpu_to_be32(make_tag());
	req.flag = USB_DIRECTION_OUT;
	req.length = 10;
	req.opcode = OPCODE_WRITE_SDRAM;
	req.address = cpu_to_be32(addr);
	req.size = cpu_to_be16(len);

	usb_bulk_send(ctx->hdl, ctx->epout, &req, sizeof(struct usb_request_t));
	usb_bulk_send(ctx->hdl, ctx->epout, buf, len);
	usb_bulk_recv(ctx->hdl, ctx->epin, &res, sizeof(struct usb_response_t));
	if((be32_to_cpu(res.signature) != USB_RESPONSE_SIGN) || (res.tag != req.tag))
		return 0;
	return 1;
}

void rock_read(struct xrock_ctx_t * ctx, uint32_t addr, void * buf, size_t len)
{
	size_t n;

	while(len > 0)
	{
		n = len > 16384 ? 16384 : len;
		rock_read_raw(ctx, addr, buf, n);
		addr += n;
		buf += n;
		len -= n;
	}
}

void rock_write(struct xrock_ctx_t * ctx, uint32_t addr, void * buf, size_t len)
{
	size_t n;

	while(len > 0)
	{
		n = len > 16384 ? 16384 : len;
		rock_write_raw(ctx, addr, buf, n);
		addr += n;
		buf += n;
		len -= n;
	}
}

void rock_read_progress(struct xrock_ctx_t * ctx, uint32_t addr, void * buf, size_t len)
{
	struct progress_t p;
	size_t n;

	progress_start(&p, len);
	while(len > 0)
	{
		n = len > 16384 ? 16384 : len;
		rock_read_raw(ctx, addr, buf, n);
		addr += n;
		buf += n;
		len -= n;
		progress_update(&p, n);
	}
	progress_stop(&p);
}

void rock_write_progress(struct xrock_ctx_t * ctx, uint32_t addr, void * buf, size_t len)
{
	struct progress_t p;
	size_t n;

	progress_start(&p, len);
	while(len > 0)
	{
		n = len > 16384 ? 16384 : len;
		rock_write_raw(ctx, addr, buf, n);
		addr += n;
		buf += n;
		len -= n;
		progress_update(&p, n);
	}
	progress_stop(&p);
}
