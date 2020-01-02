CC=gcc

CFLAGS=-g3

OFLAGS=-Ofast -funroll-loops -finline-functions

LFLAGS=-fPIC -shared

LIB_OUT=libycrypt.so

TEST_OUT=libtest

AES=aes/aes.c
RC4=rc4/rc4.c
RC5=rc5/rc5.c
RC6=rc6/rc6.c
SIMECK=simeck/simeck.c
BLOWFISH=blowfish/blowfish.c
THREEFISH=threefish/threefish.c
SERPENT=serpent/serpent.c
RED_PIKE=red_pike/red_pike.c

all: l t

l:
	$(CC) $(LFLAGS) $(CFLAGS) $(OFLAGS) $(AES) $(RC4) $(RC5) $(RC6) $(SIMECK) $(BLOWFISH) $(THREEFISH) $(SERPENT) $(RED_PIKE) -o $(LIB_OUT)

t:
	$(CC) $(CFLAGS) $(OFLAGS) -lycrypt -L. test.c -o $(TEST_OUT)

clean:
	rm -Rf *~ $(LIB_OUT) $(TEST_OUT)
