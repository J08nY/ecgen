/*-*- compile-command: "cc -c -o points.gp.o -g -O3 -Wall -fomit-frame-pointer -fno-strict-aliasing -fPIC -I"/usr/include/x86_64-linux-gnu" points.gp.c && cc -o points.gp.so -shared -g -O3 -Wall -fomit-frame-pointer -fno-strict-aliasing -fPIC -Wl,-shared -Wl,-z,relro points.gp.o -lc -lm -L/usr/lib/x86_64-linux-gnu -lpari"; -*-*/
#include <pari/pari.h>
/*
GP;install("init_points","v","init_points","./points.gp.so");
GP;install("find_point","D0,G,D0,G,D0,G,","find_point","./points.gp.so");
GP;install("find_points","D0,G,D0,G,D0,G,","find_points","./points.gp.so");
GP;install("maxprime_order","D0,G,D0,G,","maxprime_order","./points.gp.so");
GP;install("minprime_order","D0,G,D0,G,","minprime_order","./points.gp.so");
GP;install("max_order","D0,G,D0,G,","max_order","./points.gp.so");
GP;install("get_point","D0,G,D0,G,D0,G,","get_point","./points.gp.so");
GP;install("prime_orders","D0,G,D0,G,","prime_orders","./points.gp.so");
GP;install("get_points","D0,G,D0,G,D0,G,","get_points","./points.gp.so");
*/
void init_points(void);
GEN find_point(GEN e, GEN o, GEN n);
GEN find_points(GEN e, GEN o, GEN p);
GEN maxprime_order(GEN e, GEN o);
GEN minprime_order(GEN e, GEN o);
GEN max_order(GEN e, GEN o);
GEN get_point(GEN e, GEN o, GEN f);
GEN prime_orders(GEN e, GEN o);
GEN get_points(GEN e, GEN o, GEN f);
/*End of prototype*/
