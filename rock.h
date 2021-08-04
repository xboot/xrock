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

struct flash_info_t {
	uint32_t flash_size;
	uint16_t block_size;
	uint8_t page_size;
	uint8_t ecc_bits;
	uint8_t access_time;
	uint8_t manufacturer_id;
	uint8_t chip_select;
	uint8_t id[5];
};

int xrock_init(struct xrock_ctx_t * ctx);
void rock_maskrom_init_ddr(struct xrock_ctx_t * ctx, const char * filename);
void rock_maskrom_init_usbplug(struct xrock_ctx_t * ctx, const char * filename);
int rock_version(struct xrock_ctx_t * ctx, uint8_t * buf);
int rock_capability(struct xrock_ctx_t * ctx, uint8_t * buf);
int rock_reset(struct xrock_ctx_t * ctx, int maskrom);
int rock_exec(struct xrock_ctx_t * ctx, uint32_t addr);
void rock_read(struct xrock_ctx_t * ctx, uint32_t addr, void * buf, size_t len);
void rock_write(struct xrock_ctx_t * ctx, uint32_t addr, void * buf, size_t len);
void rock_read_progress(struct xrock_ctx_t * ctx, uint32_t addr, void * buf, size_t len);
void rock_write_progress(struct xrock_ctx_t * ctx, uint32_t addr, void * buf, size_t len);
int rock_flash_detect(struct xrock_ctx_t * ctx, struct flash_info_t * info);
int rock_flash_read_lba(struct xrock_ctx_t * ctx, uint32_t sec, uint32_t cnt, void * buf);
int rock_flash_read_lba_progress(struct xrock_ctx_t * ctx, uint32_t sec, uint32_t cnt, void * buf);

#ifdef __cplusplus
}
#endif

#endif /* __ROCK_H__ */
