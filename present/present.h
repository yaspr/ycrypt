#pragma once

//
#define R(v, n) (((v) >> (n)) | ((v) << (64 - n)))

//
#define F(i, n) for(i = 0; i < n; i++)

//
#define swap __builtin_bswap64

//Quadword --> 64bits
typedef unsigned long long Q;

//Byte --> 8bits
typedef unsigned char      B;

//
typedef struct present_ctx_s {

  //
  Q key[32];
  
} present_ctx;

//
void PRESENT_encrypt(void *k, void *d);
