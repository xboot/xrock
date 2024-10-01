#include <loader.h>

struct idblock0_t {
	uint32_t signature;
	uint8_t reserved0[4];
	uint32_t disable_rc4;
	uint16_t bootcode1_offset;
	uint16_t bootcode2_offset;
	uint8_t reserved1[490];
	uint16_t flash_data_size;
	uint16_t flash_boot_size;
	uint16_t crc;
};

struct idblock1_t {
	uint16_t sys_reserved_block;
	uint16_t disk0_size;
	uint16_t disk1_size;
	uint16_t disk2_size;
	uint16_t disk3_size;
	uint32_t chip_tag;
	uint32_t machine_id;
	uint16_t loader_year;
	uint16_t loader_date;
	uint16_t loader_ver;
	uint8_t reserved0[72];
	uint16_t flash_data_offset;
	uint16_t flash_data_len;
	uint8_t reserved1[384];
	uint32_t flash_chip_size;
	uint8_t reserved2;
	uint8_t access_time;
	uint16_t phy_block_size;
	uint8_t phy_page_size;
	uint8_t ecc_bits;

	uint8_t reserved3[8];
	uint16_t id_block0;
	uint16_t id_block1;
	uint16_t id_block2;
	uint16_t id_block3;
	uint16_t id_block4;
};

struct idblock2_t {
	uint16_t chip_info_size;
	uint8_t chip_info[16];
	uint8_t reserved[473];
	uint8_t sz_vc_tag[3];
	uint16_t sec0_crc;
	uint16_t sec1_crc;
	uint32_t boot_code_crc;
	uint16_t sec3_custom_data_offset;
	uint16_t sec3_custom_data_size;
	uint8_t sz_crc_tag[4];
	uint16_t sec3_crc;
};

struct idblock3_t {
	uint16_t sn_size;
	uint8_t sn[60];
	uint8_t reserved[382];
	uint8_t wifi_size;
	uint8_t wifi_addr[6];
	uint8_t imei_size;
	uint8_t imei[15];
	uint8_t uid_size;
	uint8_t uid[30];
	uint8_t bluetooth_size;
	uint8_t bluetooth_addr[6];
	uint8_t mac_size;
	uint8_t mac_addr[6];
};

static inline uint32_t loader_align_size(uint32_t len)
{
	uint32_t t = (len - 1) / 512 + 1;
	return ((t - 1) / 4 + 1) * 2048;
}

static void idb_rc4(char * buf, int len)
{
	struct rc4_ctx_t ctx;
	uint8_t key[16] = { 124, 78, 3, 4, 85, 5, 9, 7, 45, 44, 123, 56, 23, 13, 23, 17 };

	rc4_setkey(&ctx, key, sizeof(key));
	rc4_crypt(&ctx, (uint8_t *)buf, len);
}

static void rkloader_ctx_mkidb(struct rkloader_ctx_t * ctx)
{
	if(ctx->is_newidb)
	{
		ctx->idblen = 0;
		for(int i = 0; i < ctx->nentry; i++)
		{
			struct rkloader_entry_t * e = ctx->entry[i];
			if(e->type == RKLOADER_ENTRY_LOADER)
				ctx->idblen += loader_align_size(get_unaligned_le32(&e->data_size));
		}
		if(ctx->idblen > 0)
		{
			ctx->idbbuf = calloc(1, ctx->idblen);
			if(ctx->idbbuf)
			{
				int flash_head_index = 0;
				uint32_t idblen = 0;

				for(flash_head_index = 0; flash_head_index < ctx->nentry; flash_head_index++)
				{
					struct rkloader_entry_t * e = ctx->entry[flash_head_index];
					if(e->type == RKLOADER_ENTRY_LOADER)
					{
						char str[256];
						loader_wide2str(str, (uint8_t *)&e->name[0], sizeof(e->name));
						if(strcmp(str, "FlashHead") == 0)
						{
							uint32_t len = loader_align_size(get_unaligned_le32(&e->data_size));
							memset((char *)ctx->idbbuf + idblen, 0, len);
							memcpy((char *)ctx->idbbuf + idblen, (char *)ctx->buffer + get_unaligned_le32(&e->data_offset), get_unaligned_le32(&e->data_size));
							if(!ctx->is_rc4on)
							{
								for(int i = 0; i < (len / 512); i++)
									idb_rc4((char *)ctx->idbbuf + idblen + 512 * i, 512);
							}
							idblen += len;
							break;
						}
					}
				}
				for(int idx = 0; idx < ctx->nentry; idx++)
				{
					struct rkloader_entry_t * e = ctx->entry[idx];
					if((e->type == RKLOADER_ENTRY_LOADER) && (idx != flash_head_index))
					{
						uint32_t len = loader_align_size(get_unaligned_le32(&e->data_size));
						memset((char *)ctx->idbbuf + idblen, 0, len);
						memcpy((char *)ctx->idbbuf + idblen, (char *)ctx->buffer + get_unaligned_le32(&e->data_offset), get_unaligned_le32(&e->data_size));
						if(!ctx->is_rc4on)
						{
							for(int i = 0; i < (len / 512); i++)
								idb_rc4((char *)ctx->idbbuf + idblen + 512 * i, 512);
						}
						idblen += len;
					}
				}
			}
		}
	}
	else
	{
		ctx->idblen = 0;
		for(int i = 0; i < ctx->nentry; i++)
		{
			struct rkloader_entry_t * e = ctx->entry[i];
			if(e->type == RKLOADER_ENTRY_LOADER)
			{
				char str[256];
				loader_wide2str(str, (uint8_t *)&e->name[0], sizeof(e->name));
				if((strcmp(str, "FlashBoot") == 0) || (strcmp(str, "FlashData") == 0))
					ctx->idblen += loader_align_size(get_unaligned_le32(&e->data_size));
			}
		}
		if(ctx->idblen > 0)
		{
			ctx->idblen += sizeof(struct idblock0_t) + sizeof(struct idblock1_t) + sizeof(struct idblock2_t) + sizeof(struct idblock3_t);
			ctx->idbbuf = calloc(1, ctx->idblen);
			if(ctx->idbbuf)
			{
				struct rkloader_entry_t * flash_data = NULL;
				struct rkloader_entry_t * flash_boot = NULL;
				uint32_t idblen = 0;
				uint32_t len;

				for(int i = 0; i < ctx->nentry; i++)
				{
					struct rkloader_entry_t * e = ctx->entry[i];
					if(e->type == RKLOADER_ENTRY_LOADER)
					{
						char str[256];
						loader_wide2str(str, (uint8_t *)&e->name[0], sizeof(e->name));
						if(strcmp(str, "FlashData") == 0)
							flash_data = e;
						else if(strcmp(str, "FlashBoot") == 0)
							flash_boot = e;
					}
				}

				struct idblock0_t * idb0 = (struct idblock0_t *)((char *)ctx->idbbuf + idblen);
				idb0->signature = 0x0ff0aa55;
				idb0->disable_rc4 = ctx->is_rc4on ? 0 : 1;
				idb0->bootcode1_offset = 4;
				idb0->bootcode2_offset = 4;
				idb0->flash_data_size = loader_align_size(get_unaligned_le32(&flash_data->data_size)) / 512;
				idb0->flash_boot_size = loader_align_size(get_unaligned_le32(&flash_data->data_size)) / 512 + loader_align_size(get_unaligned_le32(&flash_boot->data_size)) / 512;
				idblen += 512;

				struct idblock1_t * idb1 = (struct idblock1_t *)((char *)ctx->idbbuf + idblen);
				idb1->sys_reserved_block = 0xc;
				idb1->disk0_size = 0xffff;
				idb1->chip_tag = 0x38324b52;
				idblen += 512;

				struct idblock2_t * idb2 = (struct idblock2_t *)((char *)ctx->idbbuf + idblen);
				strcpy((char *)idb2->sz_vc_tag, "VC");
				strcpy((char *)idb2->sz_crc_tag, "CRC");
				idblen += 512;

				struct idblock3_t * idb3 = (struct idblock3_t *)((char *)ctx->idbbuf + idblen);
				memset(idb3, 0, sizeof(struct idblock3_t));
				idblen += 512;

				len = loader_align_size(get_unaligned_le32(&flash_data->data_size));
				memset((char *)ctx->idbbuf + idblen, 0, len);
				memcpy((char *)ctx->idbbuf + idblen, (char *)ctx->buffer + get_unaligned_le32(&flash_data->data_offset), get_unaligned_le32(&flash_data->data_size));
				if(idb0->disable_rc4)
				{
					for(int i = 0; i < (len / 512); i++)
						idb_rc4((char *)ctx->idbbuf + idblen + 512 * i, 512);
				}
		        idblen += len;

				len = loader_align_size(get_unaligned_le32(&flash_boot->data_size));
				memset((char *)ctx->idbbuf + idblen, 0, len);
				memcpy((char *)ctx->idbbuf + idblen, (char *)ctx->buffer + get_unaligned_le32(&flash_boot->data_offset), get_unaligned_le32(&flash_boot->data_size));
				if(idb0->disable_rc4)
				{
					for(int i = 0; i < (len / 512); i++)
						idb_rc4((char *)ctx->idbbuf + idblen + 512 * i, 512);
				}
		        idblen += len;

		        idb2->sec0_crc = crc16_sum(0xffff, (const uint8_t *)idb0, 512);
		        idb2->sec1_crc = crc16_sum(0xffff, (const uint8_t *)idb1, 512);
		        idb2->sec3_crc = crc16_sum(0xffff, (const uint8_t *)idb3, 512);

		        idb_rc4((char *)idb0, sizeof(struct idblock0_t));
		        idb_rc4((char *)idb2, sizeof(struct idblock2_t));
		        idb_rc4((char *)idb3, sizeof(struct idblock3_t));
			}
		}
	}
}

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

	if(le32_to_cpu(ctx->header->tag) == 0x2052444c)
		ctx->is_newidb = 1;
	else
		ctx->is_newidb = 0;

	if(ctx->header->rc4_flag)
		ctx->is_rc4on = 0;
	else
		ctx->is_rc4on = 1;

	if(ctx->header->sign_flag == 'S')
		ctx->is_sign = 1;
	else
		ctx->is_sign = 0;

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
	uint32_t len = get_unaligned_le32(&e->data_offset) + get_unaligned_le32(&e->data_size);
	if(ctx->length != len + 4)
	{
		if(ctx->buffer)
			free(ctx->buffer);
		if(ctx)
			free(ctx);
		return NULL;
	}

	uint32_t crc32 = 0x0;
	if(crc32_sum(crc32, (const uint8_t *)ctx->buffer, len) != get_unaligned_le32((char *)ctx->buffer + len))
	{
		if(ctx->buffer)
			free(ctx->buffer);
		if(ctx)
			free(ctx);
		return NULL;
	}

	rkloader_ctx_mkidb(ctx);
	return ctx;
}

void rkloader_ctx_free(struct rkloader_ctx_t * ctx)
{
	if(ctx)
	{
		if(ctx->buffer)
			free(ctx->buffer);
		if(ctx->idbbuf)
			free(ctx->idbbuf);
		free(ctx);
	}
}
