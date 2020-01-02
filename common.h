#ifndef COMMON_H
#define COMMON_H

//
typedef unsigned char       u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

//
#define rotr64(x, r) (((x) >> (r)) | ((x) << (64 - (r))))
#define rotl64(x, r) (((x) << (r)) | ((x) >> (64 - (r))))

//
#define rotr32(x, r) (((x) >> (r)) | ((x) << (32 - (r))))
#define rotl32(x, r) (((x) << (r)) | ((x) >> (32 - (r))))

#endif
