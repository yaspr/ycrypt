#ifndef RED_PIKE_H
#define RED_PIKE_H

#include "../common.h"

//
#define RED_PIKE_MAX_KEY_LEN     8

//
#define RED_PIKE_MAX_BUFFER_LEN  4096

//
#define RED_PIKE_ROUNDS          16

//
#define RED_PIKE_MAGIC  0x9E3779B9

//
void RED_PIKE_encrypt(u32 in[2], u32 mk[2]);
void RED_PIKE_decrypt(u32 in[2], u32 mk[2]);
void RED_PIKE_cvt_key(u8 *ki, u32 ko[2]);

#endif
