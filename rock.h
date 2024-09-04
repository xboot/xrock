#ifndef __ROCK_H__
#define __ROCK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <x.h>
#include <rc4.h>
#include <crc16.h>
#include <crc32.h>
#include <misc.h>
#include <loader.h>
#include <progress.h>

enum storage_type_t {
	STORAGE_TYPE_UNKNOWN	= (0 << 0),
	STORAGE_TYPE_FLASH		= (1 << 0),
	STORAGE_TYPE_EMMC		= (1 << 1),
	STORAGE_TYPE_SD			= (1 << 2),
	STORAGE_TYPE_SD1		= (1 << 3),
	STORAGE_TYPE_SPINOR		= (1 << 9),
	STORAGE_TYPE_SPINAND	= (1 << 8),
	STORAGE_TYPE_RAM		= (1 << 6),
	STORAGE_TYPE_USB		= (1 << 7),
	STORAGE_TYPE_SATA		= (1 << 10),
	STORAGE_TYPE_PCIE		= (1 << 11),
};

struct chip_t {
	uint16_t pid;
	char * name;
};

struct xrock_ctx_t {
	libusb_context * context;
	libusb_device_handle * hdl;
	struct chip_t * chip;
	int epout;
	int epin;
	int maskrom;
};

struct flash_info_t {
	uint32_t sector_total;
	uint16_t block_size;
	uint8_t page_size;
	uint8_t ecc_bits;
	uint8_t access_time;
	uint8_t manufacturer_id;
	uint8_t chip_select;
	uint8_t id[5];
};

int xrock_init(struct xrock_ctx_t * ctx);
void rock_maskrom_upload_memory(struct xrock_ctx_t * ctx, uint32_t code, void * buf, uint64_t len, int rc4);
void rock_maskrom_upload_file(struct xrock_ctx_t * ctx, uint32_t code, const char * filename, int rc4);
int rock_ready(struct xrock_ctx_t * ctx);
int rock_version(struct xrock_ctx_t * ctx, uint8_t * buf);
int rock_capability(struct xrock_ctx_t * ctx, uint8_t * buf);
int rock_reset(struct xrock_ctx_t * ctx, int maskrom);
int rock_exec(struct xrock_ctx_t * ctx, uint32_t addr, uint32_t dtb);
int rock_read(struct xrock_ctx_t * ctx, uint32_t addr, void * buf, size_t len);
int rock_write(struct xrock_ctx_t * ctx, uint32_t addr, void * buf, size_t len);
int rock_read_progress(struct xrock_ctx_t * ctx, uint32_t addr, void * buf, size_t len);
int rock_write_progress(struct xrock_ctx_t * ctx, uint32_t addr, void * buf, size_t len);
int rock_sn_read(struct xrock_ctx_t * ctx, char * sn);
int rock_sn_write(struct xrock_ctx_t * ctx, char * sn);
enum storage_type_t rock_storage_read(struct xrock_ctx_t * ctx);
int rock_flash_detect(struct xrock_ctx_t * ctx, struct flash_info_t * info);
int rock_flash_erase_lba(struct xrock_ctx_t * ctx, uint32_t sec, uint32_t cnt);
int rock_flash_read_lba(struct xrock_ctx_t * ctx, uint32_t sec, uint32_t cnt, void * buf);
int rock_flash_write_lba(struct xrock_ctx_t * ctx, uint32_t sec, uint32_t cnt, void * buf);
int rock_flash_erase_lba_progress(struct xrock_ctx_t * ctx, uint32_t sec, uint32_t cnt);
int rock_flash_read_lba_progress(struct xrock_ctx_t * ctx, uint32_t sec, uint32_t cnt, void * buf);
int rock_flash_write_lba_progress(struct xrock_ctx_t * ctx, uint32_t sec, uint32_t cnt, void * buf);

#ifdef __cplusplus
}
#endif

#endif /* __ROCK_H__ */
