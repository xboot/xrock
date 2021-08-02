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
	int maskrom;


	int epout;
	int epin;
	struct {
		char magic[8];
		uint32_t id;
		uint32_t firmware;
		uint16_t protocol;
		uint8_t  dflag;
		uint8_t  dlength;
		uint32_t scratchpad;
		uint8_t pad[8];
	} version;
};

int xrock_init(struct xrock_ctx_t * ctx);
void rock_maskrom_init_ddr(struct xrock_ctx_t * ctx, const char * filename);
void rock_maskrom_init_usbplug(struct xrock_ctx_t * ctx, const char * filename);

#ifdef __cplusplus
}
#endif

#endif /* __ROCK_H__ */
