#ifndef UTIL_LINUX_MD5_H
#define UTIL_LINUX_MD5_H

#include <stdint.h>

#define UL_MD5LENGTH 16

typedef struct {
	uint32_t buf[4];
	uint32_t bits[2];
	unsigned char in[64];
}UL_MD5Context;

void ul_MD5Init(UL_MD5Context *context);
void ul_MD5Update(UL_MD5Context *context, unsigned char const *buf, unsigned len);
void ul_MD5Final(unsigned char digest[UL_MD5LENGTH], UL_MD5Context *context);
void ul_MD5Transform(uint32_t buf[4], uint32_t const in[16]);

void MD5Encrypt_Start(const uint8_t* str);

/*
 * This is needed to make RSAREF happy on some MS-DOS compilers.
 */
typedef struct UL_MD5Context UL_MD5_CTX;

#endif /* !UTIL_LINUX_MD5_H */