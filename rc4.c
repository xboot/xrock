#include <rc4.h>

void rc4_setkey(struct rc4_ctx_t * ctx, uint8_t * key, int len)
{
	uint8_t * s = ctx->s;
	uint8_t k[256], t;
	int i, j;

	for(i = 0; i < 256; i++)
	{
		s[i] = (uint8_t)i;
		k[i] = key[i % len];
	}
	for(i = 0, j = 0; i < 256; i++)
	{
		j = (j + s[i] + k[i]) & 0xff;
		t = s[i];
		s[i] = s[j];
		s[j] = t;
	}
	ctx->i = 0;
	ctx->j = 0;
}

void rc4_crypt(struct rc4_ctx_t * ctx, uint8_t * buf, int len)
{
	uint8_t * s = ctx->s;
	uint8_t t;
	int i, j, o;

	for(i = ctx->i, j = ctx->j, o = 0; o < len; o++)
	{
		i = (i + 1) & 0xff;
		j = (j + s[i]) & 0xff;
		t = s[i];
		s[i] = s[j];
		s[j] = t;
		buf[o] ^= s[(s[i] + s[j]) & 0xff];
	}
	ctx->i = i;
	ctx->j = j;
}
