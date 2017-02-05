/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

/**
 * Constructs an elliptic curve in the form E:
 *		y^2 = x^3 + ax + b, over a prime field
 * @param a
 * @param b
 * @param p
 * @returns elliptic curve
 */
prime_weierstrass(a:int, b:int, field:gen) = {
	return(ellinit([a,b], field));
}

/**
 * Constructs an elliptic curve in the form E:
 *		y^2 + xy = x^3 + ax + b, over a binary field.
 * @param a
 * @param b
 * @param field
 * @returns elliptic curve
 */
binary_weierstrass(a:int, b:int, field:gen) = {
	return(ellinit([1,0,0,a,b], field));
}
