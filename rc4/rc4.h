#ifndef RC4_H
#define RC4_H

#include "../common.h"

//
#define RC4_SBOX_SIZE 256

//
typedef struct RC4_context_s {

  u32 x, y;
  u8 sbox[RC4_SBOX_SIZE];

} RC4_context_t;

//
RC4_context_t *RC4_context_init(u8 *k);
void RC4_encrypt(RC4_context_t *c, u8 *p, u32 plen, u8 *q);

#endif
