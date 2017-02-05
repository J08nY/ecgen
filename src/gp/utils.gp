/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

 random_primer(range:vec) = {
 	local(p:int);
 	until(isprime(p),
 		p = randomprime(range):int;
 	);
 	return(p);
 }

/**
 * Calculates a random prime of bit size bits.
 *
 * @param bits bit size of the requested prime
 * @return random prime between 2^(bits - 1) and 2^bits
 */
random_prime(bits:small) = {
	return(random_primer([2^(bits-1), 2^bits]));
}

random_intr(range:vec) = {
	return(random(range));
}

/**
 * Generates a random integer with bit size bits.
 *
 * @param bits bit size of the requested integer
 * @return random int between 2^(bits - 1) and 2^bits
 */
random_int(bits:small) = {
	return(random_intr([2^(bits-1), 2^bits]));
}

/**
 * Converts a list to a vector.
 *
 * @param l list to convert
 * @return a vector of the lists values
 */
list_to_vec(l:list) = {
	local(v:vec, n:int);
	n = length(l);
	v = vector(n);
	for(i=1, n,
		v[i] = l[i];
	);
	return(v);
}