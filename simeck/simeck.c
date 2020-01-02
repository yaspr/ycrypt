#include "simeck.h"

//
void SIMECK_encrypt(u32 *p, u32 *mk)
{
  u32 *k = mk,*x = p;
  u64 s = 0x938BCA3083F;
  u32 t, k0, k1, k2, k3, l, r;
  
  //Load 128-bit master key
  k0 = k[0];
  k1 = k[1];
  k2 = k[2];
  k3 = k[3];
  
  //Load 64-bit plaintext 
  r = x[0];
  l = x[1];

  //
  do
    {
      //Encrypt plaintext
      r ^= rotl32(l, 1) ^ (rotl32(l, 5) & l) ^ k0;

      //Swap left & rigt
      u32 tmp = l;
      l = r;
      r = tmp;

      t = (s & 1) - 4;

      //Create next subkey
      k0 ^= rotl32(k1, 1) ^ (rotl32(k1, 5) & k1) ^ t;

      tmp = k0;
      k0 = k1;
      k1 = tmp;

      tmp = k1;
      k1 = k2;
      k2 = tmp;

      tmp = k2;
      k2 = k3;
      k3 = tmp;
      
    }
  while(s >>= 1);
  
  //
  x[0] = r;
  x[1] = l;
}
