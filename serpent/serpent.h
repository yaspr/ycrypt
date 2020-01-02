#ifndef SERPENT_H
#define SERPENT_H

#include "../common.h"

//
#define HI(b) (((b) >> 4) & 0x0F)
#define LO(b) ((b) & 0x0F)

//
void SERPENT_encrypt(u32 *p, u32 *mk);

#endif
