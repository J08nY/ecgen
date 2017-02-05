/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

/**
 * Computes primes upto some upper bound.
 *
 * @param bound an upper bound on primes
 * @return a vector of primes up to bound^2
 */
prime_upto(bound:int) = {
	local(p:list, product:int, last:int, result:vec);
	p = List();

	bound = bound^2;
	listput(p, 2);
	product = 2;
	last = 2;

	while(product < bound,
		last = nextprime(last + 1);
		listput(p, last);
		product = product * last;
	);

	result = list_to_vec(p);
	listkill(p);
	return(result);
}

/**
 *
 */
invalid(coeffs:vec, field:pol, primes:vec, bits:int) = {
	local(bs:vec, cs:vec, eq:vec, e:ell, b, n, c, o):int;
	n = length(primes);
	bs = vector(n);
	eq = coeffs;
	c = 0;

	while(c < n,
		b = random_int(bits):int;
		eq[4] = b; /* Times field? */

		iferr(e = ellinit(eq,field):ell, E, next());

		o = ellsea(e):int;
		for(i=1,n,
			if((o % primes[i]) == 0 && bs[i] == 0,
				bs[i] = b;
				cs[i] = e;
				c = c + 1;
			);
		);
	);

	return(cs);
}