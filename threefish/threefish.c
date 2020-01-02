/*
  THREEFISH CIPHER (Bruce Schneier)

*/

#include "threefish.h"

//
void THREEFISH_encrypt(u8 *p, u8 *mk)
{
  u64 c[10], r, *x = (u64 *)p, t;
  
  //AES encryption of the plaintext 240 (in decimal) 
  //using a 256-bit all-zero key
  t = 0x1BD11BDAA9FC1A22;
  
  //Initialize 256-bit key
  for (u64 i = 0; i < 4; i++)
    {
      c[i] = ((u64 *)mk)[i];
      t ^= c[i];
    }
  
  //Initialize 128-bit tweak 
  c[4] = t;

  c[5] = ((u64 *)mk)[4];
  c[6] = ((u64 *)mk)[5];
  c[7] = (c[5] ^ c[6]);
  
  //Rotation values
  c[8] = 0x203a2e190517340e;
  c[9] = 0x20160c2125283910;
    
  //72 rounds
  for(u64 i = 0; ; i++)
    {
      //Add key every 4 rounds
      if((i & 3) == 0)
	{
	  t = 0;

	  //
	  for (u64 j = 0; j < 4; j++)
	    {
	      x[j] += c[((i >> 2) + j) % 5] + t;
	      t = (j < 2) ? c[(((i >> 2) + j) % 3) + 5] : i >> 2;
	    }
	}
      
      if(i == THREEFISH_ROUNDS)
	break;

      //Mixing function
      for(u64 j = 0; j < 4; j += 2)
	{
	  r         = ((u8 *)c)[64 + ((i & 7) + (j << 2))];
	  x[j]     += x[j + 1];
	  x[j + 1]  = rotl64(x[j + 1], r);
	  x[j + 1] ^= x[j];
	}
      
      //Permute
      t    = x[1];
      x[1] = x[3];
      x[3] = t;
    }
}
