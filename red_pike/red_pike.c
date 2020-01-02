/*
  RED PIKE CIPHER (GCHQ)

  64 bit block & 64 bit key
  
*/

#include "red_pike.h"

//
void RED_PIKE_encrypt(u32 in[2], u32 mk[2])
{
  //
  u32 rk0 = mk[0];
  u32 rk1 = mk[1];

  //
  u32 s0  = in[0];
  u32 s1  = in[1];

  //
  for (u32 i = 0; i < RED_PIKE_ROUNDS; i++)
    {
      rk0 += RED_PIKE_MAGIC;
      rk1 -= RED_PIKE_MAGIC;
      
      s0 ^= rk0;
      s0 += s1;
      s0 = rotl32(s0, s1);
      
      s1 = rotr32(s1, s0);
      s1 -= s0;
      s1 ^= rk1;
    }

  //
  rk0 = s0;
  in[0] = s1;
  in[1] = rk0;
}

//
void RED_PIKE_decrypt(u32 in[2], u32 mk[2])
{
  //
  u32 dk[2] = { mk[1] - RED_PIKE_MAGIC * (RED_PIKE_ROUNDS + 1),
		mk[0] + RED_PIKE_MAGIC * (RED_PIKE_ROUNDS + 1)  };

  //
  RED_PIKE_encrypt(in, dk);
}

//
void RED_PIKE_cvt_key(u8 *ki, u32 ko[2])
{
  //
  u32 out1 = ki[0], out2 = ki[4];

  //
  out1 = (out1 << 8) | ki[1];
  out1 = (out1 << 8) | ki[2];
  out1 = (out1 << 8) | ki[3];

  //
  out2 = (out2 << 8) | ki[5];
  out2 = (out2 << 8) | ki[6];
  out2 = (out2 << 8) | ki[7];

  //
  ko[0] = out1;
  ko[1] = out2;
}

