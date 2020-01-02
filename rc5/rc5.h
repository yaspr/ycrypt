#ifndef RC5_H
#define RC5_H

#include "../common.h"

//
#define RC5_ROUNDS 12

//
#define RC5_K     (2 * (RC5_ROUNDS + 1))    

//
void RC5_encrypt(u32 p[2], u32 mk[4]);

#endif
