/*-*- compile-command: "cc -c -o sea.o -g -O3 -Wall -fomit-frame-pointer -fno-strict-aliasing -fPIC -I"/usr/include/x86_64-linux-gnu" sea.c && cc -o sea.so -shared -g -O3 -Wall -fomit-frame-pointer -fno-strict-aliasing -fPIC -Wl,-shared -Wl,-z,relro sea.o -lc -lm -L/usr/lib/x86_64-linux-gnu -lpari"; -*-*/
#include <pari/pari.h>
/*
GP;install("init_sea","v","init_sea","./sea.so");
GP;install("largest_prime","D0,G,D0,G,D0,G,p","largest_prime","./sea.so");
GP;install("smallest_prime","D0,G,D0,G,D0,G,p","smallest_prime","./sea.so");
GP;install("all_prime","D0,G,D0,G,D0,G,p","all_prime","./sea.so");
GP;install("small_pubkey","D0,G,D0,G,D0,G,p","small_pubkey","./sea.so");
GP;install("print_params","vD0,G,","print_params","./sea.so");
GP;install("print_params_pub","vD0,G,","print_params_pub","./sea.so");
*/
void init_sea(void);
GEN largest_prime(GEN p, GEN a, GEN b, long prec);
GEN smallest_prime(GEN p, GEN a, GEN b, long prec);
GEN all_prime(GEN p, GEN a, GEN b, long prec);
GEN small_pubkey(GEN p, GEN a, GEN b, long prec);
void print_params(GEN curve);
void print_params_pub(GEN curve);
/*End of prototype*/

void
init_sea(void)	  /* void */
{
  pari_sp ltop = avma;
  avma = ltop;
  return;
}

/* E(Fp): y^2 = x^3 + ax + b mod p
* @returns [p, a, b, [G.x, G.y], n, h]
* @param p
* @param a
* @param b
*/
GEN
largest_prime(GEN p, GEN a, GEN b, long prec)
{
  pari_sp ltop = avma;
  GEN e = gen_0, o = gen_0, G = gen_0;
  GEN p1 = gen_0;	  /* vec */
  GEN maxprime_point = pol_x(fetch_user_var("maxprime_point"));
  GEN p2 = gen_0;	  /* vec */
  p1 = cgetg(3, t_VEC);
  gel(p1, 1) = gcopy(a);
  gel(p1, 2) = gcopy(b);
  e = ellinit(p1, p, prec);
  o = ellsea(e, 0);
  if (gequal0(o))
  {
    avma = ltop;
    return gen_0;
  }
  G = get_point(e, o, maxprime_point);
  p2 = cgetg(8, t_VEC);
  gel(p2, 1) = gcopy(p);
  gel(p2, 2) = gcopy(a);
  gel(p2, 3) = gcopy(b);
  gel(p2, 4) = lift(gel(gel(G, 1), 1));
  gel(p2, 5) = lift(gel(gel(G, 1), 2));
  gel(p2, 6) = gcopy(gel(G, 2));
  gel(p2, 7) = gcopy(gel(G, 3));
  p2 = gerepilecopy(ltop, p2);
  return p2;
}

/* E(Fp): y^2 = x^3 + ax + b mod p
* @returns [p, a, b, G, n, h]
* @param p
* @param a
* @param b
*/
GEN
smallest_prime(GEN p, GEN a, GEN b, long prec)
{
  pari_sp ltop = avma;
  GEN e = gen_0, o = gen_0, G = gen_0;
  GEN p1 = gen_0;	  /* vec */
  GEN minprime_point = pol_x(fetch_user_var("minprime_point"));
  GEN p2 = gen_0;	  /* vec */
  p1 = cgetg(3, t_VEC);
  gel(p1, 1) = gcopy(a);
  gel(p1, 2) = gcopy(b);
  e = ellinit(p1, p, prec);
  o = ellsea(e, 0);
  if (gequal0(o))
  {
    avma = ltop;
    return gen_0;
  }
  G = get_point(e, o, minprime_point);
  p2 = cgetg(8, t_VEC);
  gel(p2, 1) = gcopy(p);
  gel(p2, 2) = gcopy(a);
  gel(p2, 3) = gcopy(b);
  gel(p2, 4) = lift(gel(gel(G, 1), 1));
  gel(p2, 5) = lift(gel(gel(G, 1), 2));
  gel(p2, 6) = gcopy(gel(G, 2));
  gel(p2, 7) = gcopy(gel(G, 3));
  p2 = gerepilecopy(ltop, p2);
  return p2;
}

/* E(Fp): y^2 = x^3 + ax + b mod p
* @returns vector of domain parameters [p, a, b, G, n, h]  points of all prime orders
* @param p
* @param a
* @param b
*/
GEN
all_prime(GEN p, GEN a, GEN b, long prec)
{
  pari_sp ltop = avma;
  GEN e = gen_0, o = gen_0, G = gen_0;
  GEN p1 = gen_0;	  /* vec */
  GEN prime_orders = pol_x(fetch_user_var("prime_orders"));
  long l2;
  GEN p3 = gen_0;	  /* vec */
  p1 = cgetg(3, t_VEC);
  gel(p1, 1) = gcopy(a);
  gel(p1, 2) = gcopy(b);
  e = ellinit(p1, p, prec);
  o = ellsea(e, 0);
  if (gequal0(o))
  {
    avma = ltop;
    return gen_0;
  }
  G = get_points(e, o, prime_orders);
  l2 = glength(G);
  {
    long X;
    p3 = cgetg(l2+1, t_VEC);
    for (X = 1; X <= l2; ++X)
    {
      GEN p4 = gen_0;	  /* vec */
      p4 = cgetg(8, t_VEC);
      gel(p4, 1) = gcopy(p);
      gel(p4, 2) = gcopy(a);
      gel(p4, 3) = gcopy(b);
      gel(p4, 4) = lift(gel(gel(gel(G, X), 1), 1));
      gel(p4, 5) = lift(gel(gel(gel(G, X), 1), 2));
      gel(p4, 6) = gcopy(gel(gel(G, X), 2));
      gel(p4, 7) = gcopy(gel(gel(G, X), 3));
      gel(p3, X) = p4;
    }
  }
  p3 = gerepilecopy(ltop, p3);
  return p3;
}

/*####################################################################*/

/* E(Fp): y^2 = x^3 + ax + b mod p
* @returns [p, a, b, G.x, G.y, r, k, P.x, P.y, n]
*/
GEN
small_pubkey(GEN p, GEN a, GEN b, long prec)
{
  pari_sp ltop = avma;
  GEN e = gen_0, o = gen_0, f = gen_0, G = gen_0, n = gen_0, h = gen_0, r = gen_0, P = gen_0;
  GEN p1 = gen_0, p2 = gen_0;	  /* vec */
  p1 = cgetg(3, t_VEC);
  gel(p1, 1) = gcopy(a);
  gel(p1, 2) = gcopy(b);
  e = ellinit(p1, p, prec);
  o = ellsea(e, 0);
  if (gequal0(o))
  {
    avma = ltop;
    return gen_0;
  }
  if (!gequal0(gisprime(o, 0)))
  {
    G = genrand(e);
    n = o;
    h = gen_1;
    P = genrand(e);
  }
  else
  {
    f = factor(o);
    f = vecsort0(f, NULL, 0);
    n = gcopy(gcoeff(f, 1, 2));
    h = gdivent(o, n);
    /*printf("%s %u %u\n", f, n, h); */
    {
      pari_sp btop = avma;
      do
      {
        G = genrand(e);
        r = ellorder(e, G, NULL);
        if (gc_needed(btop, 1))
          gerepileall(btop, 2, &G, &r);
      } while(!gequal0(gmod(r, n)));
    }
    /*printf("%s %s\n", G, r); */
    P = ellmul(e, G, gdivent(r, n));
  }
  p2 = cgetg(11, t_VEC);
  gel(p2, 1) = gcopy(p);
  gel(p2, 2) = gcopy(a);
  gel(p2, 3) = gcopy(b);
  gel(p2, 4) = lift(gel(G, 1));
  gel(p2, 5) = lift(gel(G, 2));
  gel(p2, 6) = gcopy(r);
  gel(p2, 7) = gdivent(o, r);
  gel(p2, 8) = lift(gel(P, 1));
  gel(p2, 9) = lift(gel(P, 2));
  gel(p2, 10) = gcopy(n);
  p2 = gerepilecopy(ltop, p2);
  return p2;
}

void
print_params(GEN curve)	  /* void */
{
  pari_sp ltop = avma;
  printf0("%x,%x,%x,%x,%x,%x,%x\n", mkvecn(7, gel(curve, 1), gel(curve, 2), gel(curve, 3), gel(curve, 4), gel(curve, 5), gel(curve, 6), gel(curve, 7)));
  avma = ltop;
  return;
}

void
print_params_pub(GEN curve)	  /* void */
{
  pari_sp ltop = avma;
  printf0("%x,%x,%x,%x,%x,%x,%x,%x,%x,%x\n", mkvecn(10, gel(curve, 1), gel(curve, 2), gel(curve, 3), gel(curve, 4), gel(curve, 5), gel(curve, 6), gel(curve, 7), gel(curve, 8), gel(curve, 9), gel(curve, 10)));
  avma = ltop;
  return;
}

