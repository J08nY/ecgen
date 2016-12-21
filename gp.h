/*-*- compile-command: "cc -c -o gp.gp.o -g -O3 -Wall -fomit-frame-pointer -fno-strict-aliasing -fPIC -I"/usr/include/x86_64-linux-gnu" gp.gp.c && cc -o gp.gp.so -shared -g -O3 -Wall -fomit-frame-pointer -fno-strict-aliasing -fPIC -Wl,-shared -Wl,-z,relro gp.gp.o -lc -lm -L/usr/lib/x86_64-linux-gnu -lpari"; -*-*/
#include <pari/pari.h>
/*
GP;install("init_gp","v","init_gp","./gp.gp.so");
GP;install("find_point","D0,G,D0,G,D0,G,","find_point","./gp.gp.so");
GP;install("find_points","D0,G,D0,G,D0,G,","find_points","./gp.gp.so");
GP;install("maxprime_order","D0,G,D0,G,","maxprime_order","./gp.gp.so");
GP;install("minprime_order","D0,G,D0,G,","minprime_order","./gp.gp.so");
GP;install("max_order","D0,G,D0,G,","max_order","./gp.gp.so");
GP;install("get_point","D0,G,D0,G,D0,G,","get_point","./gp.gp.so");
GP;install("prime_orders","D0,G,D0,G,","prime_orders","./gp.gp.so");
GP;install("get_points","D0,G,D0,G,D0,G,","get_points","./gp.gp.so");
GP;install("largest_prime","D0,G,D0,G,D0,G,p","largest_prime","./gp.gp.so");
GP;install("smallest_prime","D0,G,D0,G,D0,G,p","smallest_prime","./gp.gp.so");
GP;install("all_prime","D0,G,D0,G,D0,G,p","all_prime","./gp.gp.so");
GP;install("small_pubkey","D0,G,D0,G,D0,G,p","small_pubkey","./gp.gp.so");
GP;install("print_params","vD0,G,","print_params","./gp.gp.so");
GP;install("print_params_pub","vD0,G,","print_params_pub","./gp.gp.so");
*/
void init_gp(void);
GEN find_point(GEN e, GEN o, GEN n);
GEN find_points(GEN e, GEN o, GEN p);
GEN maxprime_order(GEN e, GEN o);
GEN minprime_order(GEN e, GEN o);
GEN max_order(GEN e, GEN o);
GEN get_point(GEN e, GEN o, GEN f);
GEN prime_orders(GEN e, GEN o);
GEN get_points(GEN e, GEN o, GEN f);
GEN largest_prime(GEN p, GEN a, GEN b, long prec);
GEN smallest_prime(GEN p, GEN a, GEN b, long prec);
GEN all_prime(GEN p, GEN a, GEN b, long prec);
GEN small_pubkey(GEN p, GEN a, GEN b, long prec);
void print_params(GEN curve);
void print_params_pub(GEN curve);
/*End of prototype*/
