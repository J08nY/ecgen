/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
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