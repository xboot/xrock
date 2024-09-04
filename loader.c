#include <loader.h>

char * loader_wide2str(char * str, uint8_t * wide, int len)
{
	int i;

	for(i = 0; i < len; i++)
	{
		char c = wide[i * 2 + 0] & 0xff;
		str[i] = (c && isprint(c)) ? c : '\0';
	}
	str[i] = '\0';
	return str;
}

struct rkloader_ctx_t * rkloader_ctx_alloc(const char * filename)
{
	struct rkloader_ctx_t * ctx = calloc(1, sizeof(struct rkloader_ctx_t));
	if(!ctx)
		return NULL;

	ctx->buffer = file_load(filename, &ctx->length);
	if(!ctx->buffer || ctx->length <= sizeof(struct rkloader_header_t))
	{
		if(ctx->buffer)
			free(ctx->buffer);
		if(ctx)
			free(ctx);
		return NULL;
	}

	ctx->header = (struct rkloader_header_t *)ctx->buffer;
	if((le32_to_cpu(ctx->header->tag) != 0x544f4f42) && (le32_to_cpu(ctx->header->tag) != 0x2052444c))
	{
		if(ctx->buffer)
			free(ctx->buffer);
		if(ctx)
			free(ctx);
		return NULL;
	}

	ctx->nentry = ctx->header->code471_num + ctx->header->code472_num + ctx->header->loader_num;
	if(ctx->nentry <= 0)
	{
		if(ctx->buffer)
			free(ctx->buffer);
		if(ctx)
			free(ctx);
		return NULL;
	}

	for(int i = 0; i < ctx->nentry; i++)
	{
		ctx->entry[i] = (struct rkloader_entry_t *)(ctx->buffer + sizeof(struct rkloader_header_t) + sizeof(struct rkloader_entry_t) * i);
	}

	struct rkloader_entry_t * e = ctx->entry[ctx->nentry - 1];
	uint32_t len = read_le32(&e->data_offset) + read_le32(&e->data_size);
	if(ctx->length != len + 4)
	{
		if(ctx->buffer)
			free(ctx->buffer);
		if(ctx)
			free(ctx);
		return NULL;
	}

	uint32_t crc32 = 0x0;
	if(crc32_sum(crc32, (const uint8_t *)ctx->buffer, len) != read_le32((char *)ctx->buffer + len))
	{
		if(ctx->buffer)
			free(ctx->buffer);
		if(ctx)
			free(ctx);
		return NULL;
	}

	return ctx;
}

void rkloader_ctx_free(struct rkloader_ctx_t * ctx)
{
	if(ctx)
	{
		if(ctx->buffer)
			free(ctx->buffer);
		free(ctx);
	}
}
