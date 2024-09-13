#ifndef __X_H__
#define __X_H__

#ifdef __cplusplus
extern "C" {
#endif

#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>
#include <libusb.h>

static inline uint16_t __swab16(uint16_t x)
{
	return ( (x<<8) | (x>>8) );
}

static inline uint32_t __swab32(uint32_t x)
{
	return ( (x<<24) | (x>>24) | \
		((x & (uint32_t)0x0000ff00UL)<<8) | \
		((x & (uint32_t)0x00ff0000UL)>>8) );
}

static inline uint64_t __swab64(uint64_t x)
{
	return ( (x<<56) | (x>>56) | \
		((x & (uint64_t)0x000000000000ff00ULL)<<40) | \
		((x & (uint64_t)0x0000000000ff0000ULL)<<24) | \
		((x & (uint64_t)0x00000000ff000000ULL)<< 8) | \
		((x & (uint64_t)0x000000ff00000000ULL)>> 8) | \
		((x & (uint64_t)0x0000ff0000000000ULL)>>24) | \
		((x & (uint64_t)0x00ff000000000000ULL)>>40) );
}

static inline uint32_t __swahw32(uint32_t x)
{
	return ( ((x & (uint32_t)0x0000ffffUL)<<16) | ((x & (uint32_t)0xffff0000UL)>>16) );
}

static inline uint32_t __swahb32(uint32_t x)
{
	return ( ((x & (uint32_t)0x00ff00ffUL)<<8) | ((x & (uint32_t)0xff00ff00UL)>>8) );
}

#if BYTE_ORDER == BIG_ENDIAN
#define cpu_to_le64(x)	(__swab64((uint64_t)(x)))
#define le64_to_cpu(x)	(__swab64((uint64_t)(x)))
#define cpu_to_le32(x)	(__swab32((uint32_t)(x)))
#define le32_to_cpu(x)	(__swab32((uint32_t)(x)))
#define cpu_to_le16(x)	(__swab16((uint16_t)(x)))
#define le16_to_cpu(x)	(__swab16((uint16_t)(x)))
#define cpu_to_be64(x)	((uint64_t)(x))
#define be64_to_cpu(x)	((uint64_t)(x))
#define cpu_to_be32(x)	((uint32_t)(x))
#define be32_to_cpu(x)	((uint32_t)(x))
#define cpu_to_be16(x)	((uint16_t)(x))
#define be16_to_cpu(x)	((uint16_t)(x))
#else
#define cpu_to_le64(x)	((uint64_t)(x))
#define le64_to_cpu(x)	((uint64_t)(x))
#define cpu_to_le32(x)	((uint32_t)(x))
#define le32_to_cpu(x)	((uint32_t)(x))
#define cpu_to_le16(x)	((uint16_t)(x))
#define le16_to_cpu(x)	((uint16_t)(x))
#define cpu_to_be64(x)	(__swab64((uint64_t)(x)))
#define be64_to_cpu(x)	(__swab64((uint64_t)(x)))
#define cpu_to_be32(x)	(__swab32((uint32_t)(x)))
#define be32_to_cpu(x)	(__swab32((uint32_t)(x)))
#define cpu_to_be16(x)	(__swab16((uint16_t)(x)))
#define be16_to_cpu(x)	(__swab16((uint16_t)(x)))
#endif

static inline uint16_t read_le16(void * addr)
{
	uint8_t * p = (uint8_t *)addr;
	return (uint16_t)((p[1] << 8) | (p[0] << 0));
}

static inline uint16_t read_be16(void * addr)
{
	uint8_t * p = (uint8_t *)addr;
	return (uint16_t)((p[0] << 8) | (p[1] << 0));
}

static inline uint32_t read_le32(void * addr)
{
	uint8_t * p = (uint8_t *)addr;
	return (uint32_t)((p[3] << 24) | (p[2] << 16) | (p[1] << 8) | (p[0] << 0));
}

static inline uint32_t read_be32(void * addr)
{
	uint8_t * p = (uint8_t *)addr;
	return (uint32_t)((p[0] << 24) | (p[1] << 16) | (p[2] << 8) | (p[3] << 0));
}

static inline void write_le16(void * addr, uint16_t val)
{
	uint8_t * p = (uint8_t *)addr;
	p[0] = (val >> 0) & 0xff;
	p[1] = (val >> 8) & 0xff;
}

static inline void write_be16(void * addr, uint16_t val)
{
	uint8_t * p = (uint8_t *)addr;
	p[0] = (val >> 8) & 0xff;
	p[1] = (val >> 0) & 0xff;
}

static inline void write_le32(void * addr, uint32_t val)
{
	uint8_t * p = (uint8_t *)addr;
	p[0] = (val >> 0) & 0xff;
	p[1] = (val >> 8) & 0xff;
	p[2] = (val >> 16) & 0xff;
	p[3] = (val >> 24) & 0xff;
}

static inline void write_be32(void * addr, uint32_t val)
{
	uint8_t * p = (uint8_t *)addr;
	p[0] = (val >> 24) & 0xff;
	p[1] = (val >> 16) & 0xff;
	p[2] = (val >> 8) & 0xff;
	p[3] = (val >> 0) & 0xff;
}

#define XMAP(x, ia, ib, oa, ob)		((x - ia) * (ob - oa) / (ib - ia) + oa)
#define XMIN(a, b)					({typeof(a) _amin = (a); typeof(b) _bmin = (b); (void)(&_amin == &_bmin); _amin < _bmin ? _amin : _bmin;})
#define XMAX(a, b)					({typeof(a) _amax = (a); typeof(b) _bmax = (b); (void)(&_amax == &_bmax); _amax > _bmax ? _amax : _bmax;})
#define XCLAMP(v, a, b)				XMIN(XMAX(a, v), b)

#define XFLOOR(x)					((x) > 0 ? (int)(x) : (int)((x) - 0.9999999999))
#define XROUND(x)					((x) > 0 ? (int)((x) + 0.5) : (int)((x) - 0.5))
#define XCEIL(x)					((x) > 0 ? (int)((x) + 0.9999999999) : (int)(x))
#define XDIV255(x)					((((int)(x) + 1) * 257) >> 16)

#define ARRAY_SIZE(array)			(sizeof(array) / sizeof((array)[0]))
#define X(...)						("" #__VA_ARGS__ "")

#ifdef __cplusplus
}
#endif

#endif /* __X_H__ */
