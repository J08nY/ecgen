#include "gp.h"

void
init_gp(void)	  /* void */
{
  pari_sp ltop = avma;
  avma = ltop;
  return;
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

GEN
pack_params(GEN p, GEN a, GEN b, GEN G)	  /* vec */
{
  pari_sp ltop = avma;
  GEN p1 = gen_0;	  /* vec */
  p1 = cgetg(8, t_VEC);
  gel(p1, 1) = gcopy(p);
  gel(p1, 2) = gcopy(a);
  gel(p1, 3) = gcopy(b);
  gel(p1, 4) = lift(gel(gel(G, 1), 1));
  gel(p1, 5) = lift(gel(gel(G, 1), 2));
  gel(p1, 6) = gcopy(gel(G, 2));
  gel(p1, 7) = gcopy(gel(G, 3));
  p1 = gerepilecopy(ltop, p1);
  return p1;
}

/**
* Finds random point of order n on curve e of order o.
* @returns [[P.x, P.y], n, h]
* @param e curve
* @param o curve order
* @param n desired point order
*/
GEN
find_point(GEN e, GEN o, GEN n)	  /* vec */
{
  pari_sp ltop = avma;
  GEN h = gen_0, P = gen_0;
  GEN p1 = gen_0;	  /* vec */
  h = gdivent(o, n);
  {
    pari_sp btop = avma;
    do
    {
      P = genrand(e);
      if (gc_needed(btop, 1))
        P = gerepilecopy(btop, P);
    } while(gequal0(ellmul(e, P, n)));
  }
  p1 = cgetg(4, t_VEC);
  gel(p1, 1) = gcopy(P);
  gel(p1, 2) = gcopy(n);
  gel(p1, 3) = gcopy(h);
  p1 = gerepilecopy(ltop, p1);
  return p1;
}

/**
* Finds random points of orders given by vector p.
* @returns vector of points in format [[P.x, P.y], n, h]
* @param e curve
* @param o curve order
* @param p vector of point orders
*/
GEN
find_points(GEN e, GEN o, GEN p)	  /* vec */
{
  pari_sp ltop = avma;
  long l1;
  GEN p2 = gen_0;	  /* vec */
  l1 = glength(p);
  {
    long X;
    p2 = cgetg(l1+1, t_VEC);
    for (X = 1; X <= l1; ++X)
      gel(p2, X) = find_point(e, o, gel(p, X));
  }
  p2 = gerepilecopy(ltop, p2);
  return p2;
}

/*####################################################################*/

GEN
maxprime_order(GEN e, GEN o)
{
  pari_sp ltop = avma;
  if (!gequal0(gisprime(o, 0)))
  {
    o = gerepilecopy(ltop, o);
    return o;
  }
  else
  {
    GEN p1 = gen_0;
    p1 = vecmax(factor(o));
    p1 = gerepilecopy(ltop, p1);
    return p1;
  }
  avma = ltop;
  return gen_0;
}

GEN
minprime_order(GEN e, GEN o)
{
  pari_sp ltop = avma;
  if (!gequal0(gisprime(o, 0)))
  {
    o = gerepilecopy(ltop, o);
    return o;
  }
  else
  {
    GEN p1 = gen_0;
    p1 = gcopy(gcoeff(factor(o), 1, 1));
    p1 = gerepilecopy(ltop, p1);
    return p1;
  }
  avma = ltop;
  return gen_0;
}

/*####################################################################*/

GEN
prime_orders(GEN e, GEN o)
{
  pari_sp ltop = avma;
  GEN f = gen_0;
  if (!gequal0(gisprime(o, 0)))
  {
    GEN p1 = gen_0;	  /* vec */
    p1 = cgetg(2, t_VEC);
    gel(p1, 1) = gcopy(o);
    p1 = gerepilecopy(ltop, p1);
    return p1;
  }
  else
  {
    long l2;
    GEN p3 = gen_0;	  /* vec */
    f = factor(o);
    l2 = glength(f);
    {
      long X;
      p3 = cgetg(l2+1, t_VEC);
      for (X = 1; X <= l2; ++X)
        gel(p3, X) = gcopy(gcoeff(f, X, 1));
    }
    p3 = gerepilecopy(ltop, p3);
    return p3;
  }
  avma = ltop;
  return gen_0;
}

/**
* E(Fp): y^2 = x^3 + ax + b mod p
* @returns [p, a, b, G.x, G.y, n, h], G has the largest prime order possible
* @param p
* @param a
* @param b
*/
GEN
largest_prime(GEN p, GEN a, GEN b, long prec)
{
  pari_sp ltop = avma;
  GEN e = gen_0, o = gen_0, G = gen_0;
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
  G = find_point(e, o, maxprime_order(o, gen_0));
  p2 = pack_params(p, a, b, G);
  p2 = gerepilecopy(ltop, p2);
  return p2;
}

/**
* E(Fp): y^2 = x^3 + ax + b mod p
* @returns [p, a, b, G.x, G.y, n, h], G has the smallest prime order possible
* @param p
* @param a
* @param b
*/
GEN
smallest_prime(GEN p, GEN a, GEN b, long prec)
{
  pari_sp ltop = avma;
  GEN e = gen_0, o = gen_0, G = gen_0;
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
  G = find_point(e, o, minprime_order(o, gen_0));
  p2 = pack_params(p, a, b, G);
  p2 = gerepilecopy(ltop, p2);
  return p2;
}

/**
* E(Fp): y^2 = x^3 + ax + b mod p
* @returns [p, a, b, G.x, G.y, n, h=1], G is generator of E(Fp)
* @param p
* @param a
* @param b
*/
GEN
generator(GEN p, GEN a, GEN b, long prec)
{
  pari_sp ltop = avma;
  GEN e = gen_0, o = gen_0, G = gen_0;
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
  G = find_point(e, o, o);
  p2 = pack_params(p, a, b, G);
  p2 = gerepilecopy(ltop, p2);
  return p2;
}

/**
* E(Fp): y^2 = x^3 + ax + b mod p
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
  G = find_points(e, o, prime_orders(o, gen_0));
  l2 = glength(G);
  {
    long X;
    p3 = cgetg(l2+1, t_VEC);
    for (X = 1; X <= l2; ++X)
      gel(p3, X) = pack_params(p, a, b, gel(G, X));
  }
  p3 = gerepilecopy(ltop, p3);
  return p3;
}

/*####################################################################*/

/**
* E(Fp): y^2 = x^3 + ax + b mod p
* @returns [p, a, b, G.x, G.y, r, k, P.x, P.y, n], G is generator of E(Fp), P has the smallest prime order
*/
GEN
small_pubkey(GEN p, GEN a, GEN b, long prec)
{
  pari_sp ltop = avma;
  GEN e = gen_0, o = gen_0, G = gen_0, f = gen_0, n = gen_0, r = gen_0, P = gen_0;
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
    r = o;
    P = genrand(e);
  }
  else
  {
    G = find_point(e, o, o);
    f = factor(o);
    n = gcopy(gcoeff(f, 1, 1));
    /*
    until(r % n == 0,
    G = random(e);
    r = ellorder(e, G);
    );
    */
    P = ellmul(e, G, gdivent(o, n));
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

