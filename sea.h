/*-*- compile-command: "cc -c -o sea.gp.o -g -O3 -Wall -fomit-frame-pointer -fno-strict-aliasing -fPIC -I"/usr/include/x86_64-linux-gnu" sea.gp.c && cc -o sea.gp.so -shared -g -O3 -Wall -fomit-frame-pointer -fno-strict-aliasing -fPIC -Wl,-shared -Wl,-z,relro sea.gp.o -lc -lm -L/usr/lib/x86_64-linux-gnu -lpari"; -*-*/
#include <pari/pari.h>
/*
GP;install("init_sea","v","init_sea","./sea.gp.so");
GP;install("largest_prime","D0,G,D0,G,D0,G,p","largest_prime","./sea.gp.so");
GP;install("smallest_prime","D0,G,D0,G,D0,G,p","smallest_prime","./sea.gp.so");
GP;install("all_prime","D0,G,D0,G,D0,G,p","all_prime","./sea.gp.so");
GP;install("small_pubkey","D0,G,D0,G,D0,G,p","small_pubkey","./sea.gp.so");
GP;install("print_params","vD0,G,","print_params","./sea.gp.so");
GP;install("print_params_pub","vD0,G,","print_params_pub","./sea.gp.so");
*/
void init_sea(void);
GEN largest_prime(GEN p, GEN a, GEN b, long prec);
GEN smallest_prime(GEN p, GEN a, GEN b, long prec);
GEN all_prime(GEN p, GEN a, GEN b, long prec);
GEN small_pubkey(GEN p, GEN a, GEN b, long prec);
void print_params(GEN curve);
void print_params_pub(GEN curve);
/*End of prototype*/
