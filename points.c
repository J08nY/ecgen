#include "points.h"

void
init_points(void)	  /* void */
{
  pari_sp ltop = avma;
  avma = ltop;
  return;
}

/* Finds random point of order n on curve e of order o.
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

/* Finds random points of orders given by vector p.
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

GEN
max_order(GEN e, GEN o)
{
  pari_sp ltop = avma;
  o = gerepilecopy(ltop, o);
  return o;
}

/* Finds a random point of order given by f(o).
* @returns [[P.x, P.y], n, h]
* with P being the point with order f(o).
* @param e curve
* @param o curve order
* @param f function returning the point order, \in maxprime_order,
*			minprime_order, max_order
*/
GEN
get_point(GEN e, GEN o, GEN f)	  /* vec */
{
  pari_sp ltop = avma;
  GEN p1 = gen_0;	  /* vec */
  p1 = find_point(e, o, closure_callgen1(f, o));
  p1 = gerepilecopy(ltop, p1);
  return p1;
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

/* Finds random points of orders given by f(o).
* @returns vector of points in format [[P.x, P.y], n, h]
* @param e curve
* @param o curve order
* @param f function returning a vector of point orders
*/
GEN
get_points(GEN e, GEN o, GEN f)	  /* vec */
{
  pari_sp ltop = avma;
  GEN p1 = gen_0;	  /* vec */
  p1 = find_points(e, o, closure_callgen1(f, o));
  p1 = gerepilecopy(ltop, p1);
  return p1;
}

