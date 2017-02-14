#include "gp.h"

void init_gp(void) /* void */
{
	pari_sp ltop = avma;
	avma = ltop;
	return;
}
