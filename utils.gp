print_params(curve) = {
	printf("%x,%x,%x,%x,%x,%x,%x\n", curve[1], curve[2], curve[3], curve[4], curve[5], curve[6], curve[7]);
}

print_params_pub(curve) = {
	printf("%x,%x,%x,%x,%x,%x,%x,%x,%x,%x\n", curve[1], curve[2], curve[3], curve[4], curve[5], curve[6], curve[7], curve[8], curve[9], curve[10]);
}

pack_params(p, a, b, G) = {
	return([p, a, b, lift(G[1][1]), lift(G[1][2]), G[2], G[3]]);
}
