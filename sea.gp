/**
 * E(Fp): y^2 = x^3 + ax + b mod p
 * @returns [p, a, b, G.x, G.y, n, h], G has the largest prime order possible
 * @param p
 * @param a
 * @param b
 */
largest_prime(p, a, b) = {
	local(e, o, G);
	e = ellinit([a, b], p);
	o = ellsea(e);
	if(!o, return);

	G = find_point(e, o, maxprime_order(o));
	return(pack_params(p, a, b, G));
}

/**
 * E(Fp): y^2 = x^3 + ax + b mod p
 * @returns [p, a, b, G.x, G.y, n, h], G has the smallest prime order possible
 * @param p
 * @param a
 * @param b
 */
smallest_prime(p, a, b) = {
	local(e, o, G);
	e = ellinit([a, b], p);
	o = ellsea(e);
	if(!o, return);

	G = find_point(e, o, minprime_order(o));
	return(pack_params(p, a, b, G));
}

/**
 * E(Fp): y^2 = x^3 + ax + b mod p
 * @returns [p, a, b, G.x, G.y, n, h=1], G is generator of E(Fp)
 * @param p
 * @param a
 * @param b
 */
generator(p, a, b) = {
	local(e, o, G);
	e = ellinit([a, b], p);
	o = ellsea(e);
	if(!o, return);

	G = find_point(e, o, o);
	return(pack_params(p, a, b, G));
}

/**
 * E(Fp): y^2 = x^3 + ax + b mod p
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

	G = find_points(e, o, prime_orders(o));
	return(vector(length(G),X,pack_params(p, a, b, G[X])));
}

/*####################################################################*/

/**
 * E(Fp): y^2 = x^3 + ax + b mod p
 * @returns [p, a, b, G.x, G.y, r, k, P.x, P.y, n], G is generator of E(Fp), P has the smallest prime order
 */
small_pubkey(p, a, b) = {
	local(e, o, G, f, n, r, P);
	e = ellinit([a, b], p);
	o = ellsea(e);
	if(!o, return);

	if(isprime(o),
		G = random(e);
		n = o;
		r = o;
		P = random(e);
	,
		G = find_point(e, o, o);
		f = factor(o);
		n = f[1,1];
		/*
		until(r % n == 0,
			G = random(e);
			r = ellorder(e, G);
		);
		*/
		P = ellmul(e, G, o\n);
	);

	return([p, a, b, lift(G[1]), lift(G[2]), r, o\r, lift(P[1]), lift(P[2]), n]);
}
