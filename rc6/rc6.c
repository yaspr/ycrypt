/*
  RC6 CIPHER (Ron Rivest, Matthew Robshaw, Ray Sidney, Yiqun Lisa Yin)
  
*/

#include "rc6.h"

//128 bit block & 256 bit key
void RC6_encrypt_128_256(u32 p[4], u32 mk[8])
{
  u32 *x = p, *k = mk;
  u32 X, Y, S[RC6_K], L[8]; 
  u32 A = 0xB7E15163, B = 0, C, D;

  //
  for (u32 i = 0; i < 8; i++)
    L[i] = k[i];

  //
  for (u32 i = 0; i < RC6_K; i++)
    {
      S[i] = A;
      A += 0x9E3779B9;
    }

  //
  A = 0;
  k = S;

  //
  for (u32 i = 0; i < RC6_K * 3; i++)
    {
      A = rotl32((S[i % RC6_K] + (A + B)), 3);
      S[i % RC6_K] = A;

      B = rotl32((L[i & 7] + (A + B)), (A + B));
      L[i & 7] = B;
    }

  //
  A = x[0];
  B = x[1];
  C = x[2];
  D = x[3];

  //
  B += *k++;
  D += *k++;

  //
  for (u32 i = 0; i < RC6_ROUNDS; i++)
    {
      X = rotl32((B * (B + B + 1)), 5);
      Y = rotl32((D * (D + D + 1)), 5);
      A = rotl32((A ^ X), Y) + *k++;
      C = rotl32((C ^ Y), X) + *k++;

      X = A;
      A = B;
      B = C;
      C = D;
      D = X;
    }

  A += *k++;
  C += *k++;

  x[0] = A;
  x[1] = B;
  x[2] = C;
  x[3] = D;
}
