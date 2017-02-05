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
GP;install("random_primer","G","random_primer","./gp/gp.gp.so");
GP;install("random_prime","L","random_prime","./gp/gp.gp.so");
GP;install("random_intr","G","random_intr","./gp/gp.gp.so");
GP;install("random_int","L","random_int","./gp/gp.gp.so");
GP;install("list_to_vec","G","list_to_vec","./gp/gp.gp.so");
GP;install("prime_upto","G","prime_upto","./gp/gp.gp.so");
GP;install("invalid","GGGGp","invalid","./gp/gp.gp.so");
*/
void init_gp(void);
GEN random_primer(GEN range);
GEN random_prime(long bits);
GEN random_intr(GEN range);
GEN random_int(long bits);
GEN list_to_vec(GEN l);
GEN prime_upto(GEN bound);
GEN invalid(GEN coeffs, GEN field, GEN primes, GEN bits, long prec);
/*End of prototype*/
#endif //GP_H
