#ifndef GP_H
#define GP_H
/*-*- compile-command: "cc -c -o gp/gp.gp.o -g -O3 -Wall -fomit-frame-pointer
 * -fno-strict-aliasing -fPIC -I"/usr/include/x86_64-linux-gnu" gp/gp.gp.c && cc
 * -o gp/gp.gp.so -shared -g -O3 -Wall -fomit-frame-pointer -fno-strict-aliasing
 * -fPIC -Wl,-shared -Wl,-z,relro gp/gp.gp.o -lc -lm -L/usr/lib/x86_64-linux-gnu
 * -lpari"; -*-*/
#include <pari/pari.h>
/*
GP;install("init_gp","v","init_gp","./gp/gp.gp.so");
GP;install("test","","test","./gp/gp.gp.so");
*/
void init_gp(void);
GEN test(void);
/*End of prototype*/
#endif  // GP_H
