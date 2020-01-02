#include <stdlib.h>
#include <string.h>

#include "rc4.h"

//
RC4_context_t *RC4_context_init(u8 *k)
{
  RC4_context_t *c = malloc(sizeof(RC4_context_t));
  
  //
  u32 len = strlen(k);

  //
  for (u32 i = 0; i < RC4_SBOX_SIZE; i++)
    c->sbox[i] = (u8)i;

  //
  for (u32 i = 0, j = 0; i < RC4_SBOX_SIZE; i++)
    {
      j = (j + c->sbox[i] + k[i % len]) % RC4_SBOX_SIZE;

      u32 t = c->sbox[i];
      c->sbox[i] = c->sbox[j];
      c->sbox[j] = t;
    }

  c->x = 0;
  c->y = 0;
  
  return c;
}

//
void RC4_encrypt(RC4_context_t *c, u8 *p, u32 plen, u8 *q)
{
  u8 x = (u8)c->x;
  u8 y = (u8)c->y;

  //
  for (u32 i = 0; i < plen; i++)
    {
      //
      x = (x + 1) & 255;
      y = (y + c->sbox[x]) & 255;

      //
      u8 t = c->sbox[x];
      c->sbox[x] = c->sbox[y];
      c->sbox[y] = t;

      u32 j = (c->sbox[x] + c->sbox[y]) & 255;

      //
      q[i] = p[i] ^ c->sbox[j];
    }
  
  c->x = x;
  c->y = y;
}
