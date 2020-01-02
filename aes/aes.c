#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aes.h"

//S-Box initializer - NOT USED!
void init_aes_sbox(u8 *sbox)
{
  u8 p = 1, q = 1;
  
  //Loop invariant: p * q == 1 in the Galois field
  do
    {
      //Multiply p by 3
      p = p ^ (p << 1) ^ (p & 0x80 ? 0x1B : 0);
      
      //Divide q by 3 (equals multiplication by 0xf6)
      q ^= q << 1;
      q ^= q << 2;
      q ^= q << 4;
      q ^= q & 0x80 ? 0x09 : 0;
      
      //Compute the affine transformation
      u8 xformed = q ^ ROTL8(q, 1) ^ ROTL8(q, 2) ^ ROTL8(q, 3) ^ ROTL8(q, 4);
      
      sbox[p] = xformed ^ 0x63;
    }
  while (p != 1);
 
  //0 is a special case since it has no inverse
  sbox[0] = 0x63;
}

//
void _expand_key_(u8 *in, u8 i)
{
  //
  u32 *q = (u32 *)in;

  *q = (*q >> 8) | ((*q & 0xff) << 24);
  
  //
  in[0] = sbox[in[0]];
  in[1] = sbox[in[1]];
  in[2] = sbox[in[2]];
  in[3] = sbox[in[3]];
  
  //
  in[0] ^= rcon[i];
}

//Key expansion (from 16 to 176 bytes) 
void AES_expand_key(u8 *key, u8 *keys)
{
  //Copy the first 16 bytes 
  memcpy(keys, key, AES_BLOCK_SIZE);
  
  u8 tmp[4];
  u32 nb_bytes = 16;
  u32 rcon_iter = 1;
  
  while (nb_bytes < AES_KEY_EXP_16)
    {
      for (u8 i = 0; i < 4; i++)
	tmp[i] = keys[i + nb_bytes - 4];

      if (nb_bytes % 16 == 0)
	_expand_key_(tmp, rcon_iter++);

      for (u8 i = 0; i < 4; i++)
	{
	  keys[nb_bytes] = keys[nb_bytes - 16] ^ tmp[i];
	  nb_bytes++;
	}
    }
}

//
void sub_bytes(u8 *state)
{
  for (u8 i = 0; i < AES_BLOCK_SIZE; i++)
    state[i] = sbox[state[i]];
}

//
void rev_sub_bytes(u8 *state)
{
  for (u8 i = 0; i < AES_BLOCK_SIZE; i++)
    state[i] = rsbox[state[i]];
}

//Shift row 0 left by 0 bytes
//Shift row 1 left by 1 byte
//Shift row 2 left by 2 bytes
//Shift row 3 left by 3 bytes
void shift_rows(u8 *state)
{
  u8 tmp[AES_BLOCK_SIZE];

  tmp[0] = state[0];
  tmp[1] = state[5];
  tmp[2] = state[10];
  tmp[3] = state[15];

  tmp[4] = state[4];
  tmp[5] = state[9];
  tmp[6] = state[14];
  tmp[7] = state[3];

  tmp[8]  = state[8];
  tmp[9]  = state[13];
  tmp[10] = state[2];
  tmp[11] = state[7];

  tmp[12] = state[12];
  tmp[13] = state[1];
  tmp[14] = state[6];
  tmp[15] = state[11];

  memcpy(state, tmp, AES_BLOCK_SIZE);
}

//
void rev_shift_rows(u8 *state)
{
  u8 tmp[AES_BLOCK_SIZE];

  tmp[0]  = state[0];
  tmp[1]  = state[13];
  tmp[2]  = state[10];
  tmp[3]  = state[7];

  tmp[4]  = state[4];
  tmp[5]  = state[1];
  tmp[6]  = state[14];
  tmp[7]  = state[11];

  tmp[8]  = state[8];
  tmp[9]  = state[5];
  tmp[10] = state[2];
  tmp[11] = state[15];

  tmp[12] = state[12];
  tmp[13] = state[9];
  tmp[14] = state[6];
  tmp[15] = state[3];

  memcpy(state, tmp, AES_BLOCK_SIZE);
}

//
void mix_columns(u8 *state)
{
  u8 tmp[AES_BLOCK_SIZE];
  
  tmp[0] = (u8)(mul2[state[0]] ^ mul3[state[1]] ^ state[2]       ^ state[3]);
  tmp[1] = (u8)(state[0]       ^ mul2[state[1]] ^ mul3[state[2]] ^ state[3]);
  tmp[2] = (u8)(state[0]       ^ state[1]       ^ mul2[state[2]] ^ mul3[state[3]]);
  tmp[3] = (u8)(mul3[state[0]] ^ state[1]       ^ state[2]       ^ mul2[state[3]]);
  
  tmp[4] = (u8)(mul2[state[4]] ^ mul3[state[5]] ^ state[6]       ^ state[7]);
  tmp[5] = (u8)(state[4]       ^ mul2[state[5]] ^ mul3[state[6]] ^ state[7]);
  tmp[6] = (u8)(state[4]       ^ state[5]       ^ mul2[state[6]] ^ mul3[state[7]]);
  tmp[7] = (u8)(mul3[state[4]] ^ state[5]       ^ state[6]       ^ mul2[state[7]]);
  
  tmp[8]  = (u8)(mul2[state[8]]  ^ mul3[state[9]]  ^ state[10]       ^ state[11]);
  tmp[9]  = (u8)(state[8]        ^ mul2[state[9]]  ^ mul3[state[10]] ^ state[11]);
  tmp[10] = (u8)(state[8]        ^ state[9]        ^ mul2[state[10]] ^ mul3[state[11]]);
  tmp[11] = (u8)(mul3[state[8]]  ^ state[9]        ^ state[10]       ^ mul2[state[11]]);
  
  tmp[12] = (u8)(mul2[state[12]] ^ mul3[state[13]] ^ state[14]       ^ state[15]);
  tmp[13] = (u8)(state[12]       ^ mul2[state[13]] ^ mul3[state[14]] ^ state[15]);
  tmp[14] = (u8)(state[12]       ^ state[13]       ^ mul2[state[14]] ^ mul3[state[15]]);
  tmp[15] = (u8)(mul3[state[12]] ^ state[13]       ^ state[14]       ^ mul2[state[15]]);

  memcpy(state, tmp, AES_BLOCK_SIZE);
}

//
void rev_mix_columns(u8 *state)
{
  u8 tmp[AES_BLOCK_SIZE];

  tmp[0] = (u8) (mul14[state[0]] ^ mul11[state[1]] ^ mul13[state[2]] ^ mul9[state[3]]);
  tmp[1] = (u8) (mul9[state[0]] ^ mul14[state[1]] ^ mul11[state[2]] ^ mul13[state[3]]);
  tmp[2] = (u8) (mul13[state[0]] ^ mul9[state[1]] ^ mul14[state[2]] ^ mul11[state[3]]);
  tmp[3] = (u8) (mul11[state[0]] ^ mul13[state[1]] ^ mul9[state[2]] ^ mul14[state[3]]);
 
  tmp[4] = (u8) (mul14[state[4]] ^ mul11[state[5]] ^ mul13[state[6]] ^ mul9[state[7]]);
  tmp[5] = (u8) (mul9[state[4]] ^ mul14[state[5]] ^ mul11[state[6]] ^ mul13[state[7]]);
  tmp[6] = (u8) (mul13[state[4]] ^ mul9[state[5]] ^ mul14[state[6]] ^ mul11[state[7]]);
  tmp[7] = (u8) (mul11[state[4]] ^ mul13[state[5]] ^ mul9[state[6]] ^ mul14[state[7]]);
 
  tmp[8] = (u8) (mul14[state[8]] ^ mul11[state[9]] ^ mul13[state[10]] ^ mul9[state[11]]);
  tmp[9] = (u8) (mul9[state[8]] ^ mul14[state[9]] ^ mul11[state[10]] ^ mul13[state[11]]);
  tmp[10] = (u8) (mul13[state[8]] ^ mul9[state[9]] ^ mul14[state[10]] ^ mul11[state[11]]);
  tmp[11] = (u8) (mul11[state[8]] ^ mul13[state[9]] ^ mul9[state[10]] ^ mul14[state[11]]);
 
  tmp[12] = (u8) (mul14[state[12]] ^ mul11[state[13]] ^ mul13[state[14]] ^ mul9[state[15]]);
  tmp[13] = (u8) (mul9[state[12]] ^ mul14[state[13]] ^ mul11[state[14]] ^ mul13[state[15]]);
  tmp[14] = (u8) (mul13[state[12]] ^ mul9[state[13]] ^ mul14[state[14]] ^ mul11[state[15]]);
  tmp[15] = (u8) (mul11[state[12]] ^ mul13[state[13]] ^ mul9[state[14]] ^ mul14[state[15]]);

  memcpy(state, tmp, AES_BLOCK_SIZE);
}

//
void add_round_key(u8 *state, u8 *key)
{
  for (u8 i = 0; i < AES_BLOCK_SIZE; i++)
    state[i] ^= key[i];
}

//
void AES_encrypt_128(u8 *in, u8 *keys, u8 *out)
{
  u8 state[AES_BLOCK_SIZE];
  u32 nb_rounds = AES_NB_ROUNDS;

  memcpy(state, in, AES_BLOCK_SIZE);
  
  add_round_key(state, keys);
  
  //Rounds
  for (u8 i = 0; i < nb_rounds; i++)
    {
      sub_bytes(state);
      shift_rows(state);
      mix_columns(state);
      add_round_key(state, keys + (AES_BLOCK_SIZE * (i + 1))); //each round has a key
    }

  //Final round
  sub_bytes(state);
  shift_rows(state);
  add_round_key(state, keys + (AES_KEY_EXP_16 - AES_KEY_16)); //Last key

  memcpy(out, state, AES_BLOCK_SIZE);
}

//
void AES_decrypt_128(u8 *in, u8 *keys, u8 *out)
{
  u8 state[AES_BLOCK_SIZE];
  u32 nb_rounds = AES_NB_ROUNDS;

  memcpy(state, in, AES_BLOCK_SIZE);
  
  add_round_key(state, keys + (AES_KEY_EXP_16 - AES_KEY_16));

  for (u8 i = nb_rounds; i > 0; i--)
    {
      rev_shift_rows(state);
      rev_sub_bytes(state);
      add_round_key(state, keys + (AES_BLOCK_SIZE * i));
      rev_mix_columns(state);
    }
  
  rev_shift_rows(state);
  rev_sub_bytes(state);
  add_round_key(state, keys); 

  memcpy(out, state, AES_BLOCK_SIZE);
}

