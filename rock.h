#ifndef __ROCK_H__
#define __ROCK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <x.h>
#include <rc4.h>
#include <crc16.h>
#include <progress.h>

struct chip_t {
	uint16_t pid;
	char * name;
};

struct xrock_ctx_t {
	libusb_device_handle * hdl;
	struct chip_t * chip;
	int epout;
	int epin;
	int maskrom;
};

enum reset_type_t {
	RESET_TYPE_NORMAL		= 0x00,
	RESET_TYPE_MSC			= 0x01,
	RESET_TYPE_POWEROFF		= 0x02,
	RESET_TYPE_MASKROM		= 0x03,
	RESET_TYPE_DISCONNECT	= 0x04,
};

int xrock_init(struct xrock_ctx_t * ctx);
void rock_maskrom_init_ddr(struct xrock_ctx_t * ctx, const char * filename);
void rock_maskrom_init_usbplug(struct xrock_ctx_t * ctx, const char * filename);
int rock_reset(struct xrock_ctx_t * ctx, enum reset_type_t type);
int rock_exec(struct xrock_ctx_t * ctx, uint32_t addr);
void rock_read(struct xrock_ctx_t * ctx, uint32_t addr, void * buf, size_t len);
void rock_write(struct xrock_ctx_t * ctx, uint32_t addr, void * buf, size_t len);
void rock_read_progress(struct xrock_ctx_t * ctx, uint32_t addr, void * buf, size_t len);
void rock_write_progress(struct xrock_ctx_t * ctx, uint32_t addr, void * buf, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* __ROCK_H__ */
