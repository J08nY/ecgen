/**
 * Finds random point of order n on curve e of order o.
 * @returns [[P.x, P.y], n, h]
 * @param e curve
 * @param o curve order
 * @param n desired point order
 */
find_point(e, o, n) = {
	local(h, P);
	h = o\n;
	until(ellmul(e, P, n),
		P = random(e);
	);
	return([P, n, h]);
}

/**
 * Finds random points of orders given by vector p.
 * @returns vector of points in format [[P.x, P.y], n, h]
 * @param e curve
 * @param o curve order
 * @param p vector of point orders
 */
find_points(e, o, p) = {
	return(vector(length(p),X,find_point(e, o, p[X])));
}

/*####################################################################*/

maxprime_order(e, o) = {
	if(isprime(o),
		return(o);
	,
		return(vecmax(factor(o)));
	);
}

minprime_order(e, o) = {
	if(isprime(o),
		return(o);
	,
		return(factor(o)[1,1]);
	);
}

/*####################################################################*/

prime_orders(e, o) = {
	local(f);
	if(isprime(o),
		return([o]);
	,
		f = factor(o);
		return(vector(length(f),X,f[X,1]));
	);
}
