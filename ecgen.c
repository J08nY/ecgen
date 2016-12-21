/**
 * @author J08nY
 * @license GPL v2.0
 * @version 0.1
 */

#include "gp.h"

#include <pari/pari.h>

#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 *
 *
 */
char *readHex(const char* prompt) {
    char *r = readline(prompt);
    size_t len = strlen(r);
    if (len > 2 && r[0] == '0' && (r[1] == 'x' || r[1] == 'X')){
	return r;
    }

    char *result = realloc(r, len + 3);
    memmove(result + 2, result, len + 1);
    result[0] = '0';
    result[1] = 'x';
    return result;
}

/**
 *
 *
 *
 */
int main(int argc, char * argv[]) {
	GEN (*todo)(GEN,GEN,GEN,long) = &largest_prime;
	void (*print)(GEN) = &print_params;

	if (argc > 1) {
		if (!strcmp(argv[1], "-l")) {
		todo = &largest_prime;
		print = &print_params;
		} else if (!strcmp(argv[1], "-s")) {
		todo = &smallest_prime;
		print = &print_params;
		} else if (!strcmp(argv[1], "-p")) {
		todo = &small_pubkey;
		print = &print_params_pub;
		}
	}

	init_gp();
	pari_init( 1e9, 1e8 );

	//default0("datadir","./data");

	char *sp = readHex("p:");
	char *sa = readHex("a:");
	char *sb = readHex("b:");

	GEN p = strtoi(sp);
	GEN a = strtoi(sa);
	GEN b = strtoi(sb);
	free(sp);
	free(sa);
	free(sb);

	GEN result = (*todo)(p, a, b, 0);
	(*print)(result);
	pari_close();
	return 0;
}
