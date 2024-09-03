#include <rkboot.h>

char * wide2str(char * str, uint8_t * wide, int len)
{
	char c;
	int i;

	for(i = 0; i < len; i++)
	{
		c = wide[i * 2 + 0] & 0xff;
		str[i] = (c && isprint(c)) ? c : '\0';
	}
	str[i] = '\0';
	return str;
}

struct rkboot_ctx_t * rkboot_ctx_alloc(const char * filename)
{
	struct rkboot_ctx_t * ctx = calloc(1, sizeof(struct rkboot_ctx_t));
	if(!ctx)
		return NULL;

	ctx->buffer = file_load(filename, &ctx->length);
	if(!ctx->buffer || ctx->length <= sizeof(struct rkboot_header_t))
	{
		if(ctx->buffer)
			free(ctx->buffer);
		if(ctx)
			free(ctx);
		return NULL;
	}

	ctx->header = (struct rkboot_header_t *)ctx->buffer;
	if((le32_to_cpu(ctx->header->tag) != 0x544f4f42) && (le32_to_cpu(ctx->header->tag) != 0x2052444c))
	{
		if(ctx->buffer)
			free(ctx->buffer);
		if(ctx)
			free(ctx);
		return NULL;
	}
	ctx->nentry = ctx->header->code471_num + ctx->header->code472_num + ctx->header->loader_num;
	for(int i = 0; i < ctx->nentry; i++)
	{
		ctx->entry[i] = (struct rkboot_entry_t *)(ctx->buffer + sizeof(struct rkboot_header_t) + sizeof(struct rkboot_entry_t) * i);
	}

	return ctx;
}

void rkboot_ctx_free(struct rkboot_ctx_t * ctx)
{
	if(ctx)
	{
		if(ctx->buffer)
			free(ctx->buffer);
		free(ctx);
	}
}
