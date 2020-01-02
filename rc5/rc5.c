/*
  RC5 CIPHER (Ron Rivest)
  
*/

#include "rc5.h"

//64 bit block & 128 bit key 
void RC5_encrypt(u32 p[2], u32 mk[4])
{
  u32 *x = p, *K = mk;
  u32 A = 0xB7E15163, B = 0;
  u32 *k, X, S[RC5_K], L[4];
  
  //
  for (u8 i = 0; i < 4; i++)
    L[i] = K[i];

  //Initialize S
  for (u8 i = 0; i < RC5_K; i++)
    {
      S[i] = A;
      A += 0x9E3779B9;
    }
  
  //
  A = 0;
  k = S;

  //Create subkeys
  for (u32 i = 0; i < 3 * RC5_K; i++)
    {
      A = rotl32((S[i % RC5_K] + (A + B)), 3);
      S[i % RC5_K] = A;

      B = rotl32((L[i & 3]     + (A + B)), (A + B));
      L[i & 3]     = B;
    }

  //Add subkeys to first 64 bit block of plaintext
  A = x[0] + *k++;
  B = x[1] + *k++;

  //Do rounds
  for (u32 i = 0; i < RC5_K - 2; i++)
    {
      X = rotl32((A ^ B), B & 255) + *k++;
      A = B;
      B = X;
    }

  //Output
  x[0] = A;
  x[1] = B;
}
