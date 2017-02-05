/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

/**
 * Extract a field representation from a field.
 *  - char(field) == 2:
 *    returns the vector of powers of middle coefficients of the reduction polynomial.
 *  - char(field) != 2:
 *     returns the field characteristic(p).
 *
 * @return field representation
 */
field_params(field:gen) = {
	if(type(field) == "t_INT",
		return([field]);
	);

	local(out:vec, j:int, c:int);
	out = vector(3);

	j = 1;
	for(i=2, length(field.mod) - 2,
		c = polcoeff(field.mod, i):int;
		if(c != 0,
			out[j] = i;
			j++;
		);
	);

	return(out);
}