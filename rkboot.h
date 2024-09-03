#ifndef __RKBOOT_H__
#define __RKBOOT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <x.h>
#include <misc.h>

enum rkboot_entry_type_t {
	RKBOOT_ENTRY_471	= (1 << 0),
	RKBOOT_ENTRY_472	= (1 << 1),
	RKBOOT_ENTRY_LOADER	= (1 << 2),
};

struct rkboot_time_t {
	uint16_t	year;
	uint8_t		month;
	uint8_t		day;
	uint8_t		hour;
	uint8_t		minute;
	uint8_t		second;
} __attribute__((packed));

struct rkboot_header_t {
	uint32_t	tag;
	uint16_t	size;
	uint32_t	version;
	uint32_t	merger_version;
	struct rkboot_time_t release_time;
	uint32_t	chiptype;
	uint8_t		code471_num;
	uint32_t	code471_offset;
	uint8_t		code471_size;
	uint8_t		code472_num;
	uint32_t	code472_offset;
	uint8_t		code472_size;
	uint8_t		loader_num;
	uint32_t	loader_offset;
	uint8_t		loader_size;
	uint8_t		sign_flag;
	uint8_t		rc4_flag;
	uint8_t		reserved[57];
} __attribute__((packed));

struct rkboot_entry_t {
	uint8_t		size;
	uint32_t	type;
	uint16_t	name[20];
	uint32_t	data_offset;
	uint32_t	data_size;
	uint32_t	data_delay;
} __attribute__((packed));

struct rkboot_ctx_t {
	void * buffer;
	uint64_t length;
	struct rkboot_header_t * header;
	struct rkboot_entry_t * entry[32];
	int nentry;
};

char * wide2str(char * str, uint8_t * wide, int len);
struct rkboot_ctx_t * rkboot_ctx_alloc(const char * filename);
void rkboot_ctx_free(struct rkboot_ctx_t * ctx);

#ifdef __cplusplus
}
#endif

#endif /* __RKBOOT_H__ */
