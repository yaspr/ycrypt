#ifndef RC6_H
#define RC6_H

#include "../common.h"

//
#define RC6_ROUNDS 20

//
#define RC6_K      44 //2 * (RC6_ROUNDS + 2)

//
void RC6_encrypt_128_256(u32 p[4], u32 mk[8]);

#endif
