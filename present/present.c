//
#include <stdio.h>
#include <string.h>

#include "present.h"

//[H - 4bits][L - 4bits] ==> 1 byte
B S(B b)
{
  static const B sbox[16] = {
    0x0C, 0x05, 0x06, 0x0B,
    0x09, 0x00, 0x0A, 0x0D,
    0x03, 0x0E, 0x0F, 0x08,
    0x04, 0x07, 0x01, 0x02 };
  
  return (sbox[((b & 0xF0) >> 4)] << 4) | (sbox[b & 0x0F]);
}

//
void PRESENT_encrypt(void *k, void *d)
{
  //
  Q i, j, r, p, t, k0, k1;
  
  //
  Q *pk = (Q *)k;
  Q *pd = (Q *)d;
  
  //128 bits from key
  k0 = swap(pk[0]);
  k1 = swap(pk[1]);

  //64 bits from clear text
  t = swap(pd[0]);

  //31 rounds of the algorithm
  F(i, 31)
    {
      //Mix the key
      p = k0 ^ t;
      
      //Non-linear layer
      F(j, 8)
	((B *)&p)[j] = S(((B *)&p)[j]);
      
      //Linear layer
      t = 0;
      r = 0x0030002000100000; //0, 16, 32, 48, ...
      
      F(j, 64)
	{
	  t |= ((p >> j) & 0x01) << (r & 0xFF);
	  r = R(r + 1, 16);
	}
      	
      //Subkey creation
      p  = (k0 << 61) | (k1 >> 3);
      k1 = (k1 << 61) | (k0 >> 3);

      p = R(p, 56);
      
      //Non-linear layer
      ((B *)&p)[0] = S(((B *)&p)[0]);
      k0 = R(p, 8) ^ ((i + 1) >> 2);
      k1 ^= (((i + 1) & 3) << 62); 
    }
  
  //Mix key
  pd[0] = swap(t ^ k0);
}
