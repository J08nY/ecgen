/* E(Fp): y^2 = x^3 + ax + b mod p
 * @returns [p, a, b, [G.x, G.y], n, h]
 * @param p
 * @param a
 * @param b
 */
largest_prime(p, a, b) = {
	local(e, o, G);
	e = ellinit([a, b], p);
	o = ellsea(e);
	if(!o, return);

	G = get_point(e, o, maxprime_point);
	return([p, a, b, lift(G[1][1]), lift(G[1][2]), G[2], G[3]]);
}

/* E(Fp): y^2 = x^3 + ax + b mod p
 * @returns [p, a, b, G, n, h]
 * @param p
 * @param a
 * @param b
 */
smallest_prime(p, a, b) = {
	local(e, o, G);
	e = ellinit([a, b], p);
	o = ellsea(e);
	if(!o, return);

	G = get_point(e, o, minprime_point);
	return([p, a, b, lift(G[1][1]), lift(G[1][2]), G[2], G[3]]);
}

/* E(Fp): y^2 = x^3 + ax + b mod p
 * @returns vector of domain parameters [p, a, b, G, n, h]  points of all prime orders
 * @param p
 * @param a
 * @param b
 */
all_prime(p, a, b) = {
	local(e, o, G);
	e = ellinit([a, b], p);
	o = ellsea(e);
	if(!o, return);

	G = get_points(e, o, prime_orders);
	return(vector(length(G),X,[p, a, b, lift(G[X][1][1]), lift(G[X][1][2]), G[X][2], G[X][3]]));
}

/*####################################################################*/

/* E(Fp): y^2 = x^3 + ax + b mod p
 * @returns [p, a, b, G.x, G.y, r, k, P.x, P.y, n]
 */
small_pubkey(p,a,b) =
{
	local(e, o, f, G, n, h, r, P);
	e = ellinit([a,b],p);
	o = ellsea(e);
	if(!o, return);

	if(isprime(o),
		G = random(e);
		n = o;
		h = 1;
		P = random(e);
		,
		f = factor(o);
		f = vecsort(f);
		n = f[1,2];
		h = o\n;

		\\printf("%s %u %u\n", f, n, h);
		until(r % n == 0,
			G = random(e);
			r = ellorder(e, G);
			\\printf("%s %s\n", G, r);
		);
		P = ellmul(e,G,r\n);
	);

	return([p,a,b,lift(G[1]),lift(G[2]),r,o\r,lift(P[1]),lift(P[2]),n]);
}

print_params(curve) =
{
	printf("%x,%x,%x,%x,%x,%x,%x\n", curve[1], curve[2], curve[3], curve[4], curve[5], curve[6], curve[7]);
}

print_params_pub(curve) =
{
	printf("%x,%x,%x,%x,%x,%x,%x,%x,%x,%x\n", curve[1], curve[2], curve[3], curve[4], curve[5], curve[6], curve[7], curve[8], curve[9], curve[10]);
}
