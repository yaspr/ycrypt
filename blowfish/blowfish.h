#ifndef BLOWFISH_H
#define BLOWFISH_H

#include "../common.h"

//
#define BLOWFISH_BLOCK       8
#define BLOWFISH_ROUNDS     16
#define BLOWFISH_KEY_MIN     4
#define BLOWFISH_KEY_MAX    56
#define BLOWFISH_SBOX_SIZE 256

//
#define BLOWFISH_DECRYPT 0
#define BLOWFISH_ENCRYPT 1

//
#define SWAP32(v) ((rotl32(v,  8) & 0x00FF00FF) | (rotl32(v, 24) & 0xFF00FF00))

//
typedef struct BLOWFISH_context_s {
  
  u32 p[BLOWFISH_ROUNDS + 2];

  u32 sbox1[BLOWFISH_SBOX_SIZE];
  u32 sbox2[BLOWFISH_SBOX_SIZE];
  u32 sbox3[BLOWFISH_SBOX_SIZE];
  u32 sbox4[BLOWFISH_SBOX_SIZE];

} BLOWFISH_context_t;

//
void BLOWFISH_encrypt(u32 *p, BLOWFISH_context_t *k);
void BLOWFISH_decrypt(u32 *c, BLOWFISH_context_t *k);
BLOWFISH_context_t *BLOWFISH_context_init(u8 *k, u32 len);
void BLOWFISH_ecb_process(BLOWFISH_context_t *c, u8 *p, u8 *q, u8 mode);

#endif
