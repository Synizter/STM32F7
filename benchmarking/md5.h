#ifndef __MD5_H__
#define __MD5_H__

#include <stdint.h>

#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))

void md5_test(uint8_t*, uint8_t*);

#endif