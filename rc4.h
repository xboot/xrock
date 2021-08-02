#ifndef __RC4_H__
#define __RC4_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <x.h>

struct rc4_ctx_t {
	uint8_t s[256];
	int i, j;
};

void rc4_setkey(struct rc4_ctx_t * ctx, uint8_t * key, int len);
void rc4_crypt(struct rc4_ctx_t * ctx, uint8_t * buf, int len);

#ifdef __cplusplus
}
#endif

#endif /* __RC4_H__ */
