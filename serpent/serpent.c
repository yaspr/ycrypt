/*
  SERPENT CIPHER (Eli Biham, Ross Anderson, Lars Knudsen)
*/

#include "serpent.h"

//
void SERPENT_sbox(u32 *x, u32 idx)
{
  u8 s[16], p[16], t, c;
  
  static u8 sbox[8][8] = { { 0x83, 0x1F, 0x6A, 0xB5, 0xDE, 0x24, 0x07, 0xC9 },
			   { 0xCF, 0x72, 0x09, 0xA5, 0xB1, 0x8E, 0xD6, 0x43 },
			   { 0x68, 0x97, 0xC3, 0xFA, 0x1D, 0x4E, 0xB0, 0x25 },
			   { 0xF0, 0x8B, 0x9C, 0x36, 0x1D, 0x42, 0x7A, 0xE5 },
			   { 0xF1, 0x38, 0x0C, 0x6B, 0x52, 0xA4, 0xE9, 0xD7 },
			   { 0x5F, 0xB2, 0xA4, 0xC9, 0x30, 0x8E, 0x6D, 0x17 },
			   { 0x27, 0x5C, 0x48, 0xB6, 0x9E, 0xF1, 0x3D, 0x0A },
			   { 0xD1, 0x0F, 0x8E, 0xB2, 0x47, 0xAC, 0x39, 0x65 } };

  //
  for(u32 i = 0; i < 16; i += 2)
    {
      t = sbox[idx & 7][i >> 1];

      s[i    ] = LO(t);
      s[i + 1] = HI(t);
    }
    
  //Initial permutation
  for (u8 i = 0; i < 16; i++)
    for (u8 j = 0; j < 8; j++)
      {
	c = x[j & 3] & 1;
	x[j & 3] >>= 1;
	p[i] = (c << 7) | (p[i] >> 1);
      }

  //
  for (u8 i = 0; i < 16; i++)
    p[i] = (s[HI(p[i])] << 4) | s[LO(p[i])];

  //Final permutation
  for (u8 i = 0; i < 4; i++)
    for (u8 j = 0; j < 32; j++)
      {
	c = ((u32 *)p)[i] & 1;
	((u32 *)p)[i] >>= 1;
	x[j & 3] = (c << 31) | (x[j & 3] >> 1);
      }
}

//
void SERPENT_encrypt(u32 *p, u32 *mk)
{
  //
  u32 rk[4], k[8], *x = p;

  //
  for (u32 i = 0; i < 8; i++)
    k[i] = ((u32 *)mk)[i];

  //
  for(u32 i = 0; ; )
    {
      for (u32 j = 0; j < 4; j++)
	{
	  rk[j] = rotr32((k[0] ^ k[3] ^ k[5] ^ k[7] ^ 0x9e3779b9 ^ (i * 4 + j)), 21);

	  for (u32 s = 0; s < 7; s++)
	    k[s] = k[s + 1];

	  k[7] = rk[j];
	}

      //
      SERPENT_sbox(rk, 3 - i);

      //
      x[0] ^= rk[0];
      x[1] ^= rk[1];
      
      x[2] ^= rk[2];
      x[3] ^= rk[3];

      //
      if(i == 32)
	break;

      //
      SERPENT_sbox(x, i);

      //
      if(++i != 32)
	{
	  x[0] =  rotr32(x[0], 19);
	  x[2] =  rotr32(x[2], 29);
	  
	  x[1] ^= x[0] ^ x[2];
	  x[3] ^= x[2] ^ (x[0] << 3);
	  
	  x[1] =  rotr32(x[1], 31);
	  x[3] =  rotr32(x[3], 25);

	  x[0] ^= x[1] ^ x[3];
	  x[2] ^= x[3] ^ (x[1] << 7);
	  
	  x[0] =  rotr32(x[0], 27);
	  x[2] =  rotr32(x[2], 10);
	}
    }
}

