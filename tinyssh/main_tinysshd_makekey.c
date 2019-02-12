/*
20121022
Jan Mojzis
Public domain.
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "savesync.h"
#include "log.h"
#include "sshcrypto.h"
#include "global.h"
#include "die.h"
#include "main.h"

#define sk global_bspace1 /* reusing global buffer */
#define pk global_bspace2 /* reusing global buffer */

#define USAGEPARAMS "keydir"

static void create(const char *d, const char *fn, const unsigned char *x, long long xlen) {
    if (savesync(fn, x, xlen) == -1) die_fatal("unable to create", d, fn);
}

static int flagverbose = 1;

int main_tinysshd_makekey(int argc, char **argv, const char *binaryname) {

    char *x;
    long long i;

    log_init(flagverbose, binaryname, 0, 0);

    if (argc < 2) die_usage(binaryname, USAGEPARAMS);
    if (!argv[0]) die_usage(binaryname, USAGEPARAMS);
    for (;;) {
        if (!argv[1]) break;
        if (argv[1][0] != '-') break;
        x = *++argv;
        if (x[0] == '-' && x[1] == 0) break;
        if (x[0] == '-' && x[1] == '-' && x[2] == 0) break;
        while (*++x) {
            if (*x == 'q') { flagverbose = 0; continue; }
            if (*x == 'Q') { flagverbose = 1; continue; }
            die_usage(binaryname, USAGEPARAMS);
        }
    }
    x = *++argv; if (!x) die_usage(binaryname, USAGEPARAMS);

    log_init(flagverbose, binaryname, 0, 0);

    umask(022);
    if (mkdir(x, 0755) == -1) die_fatal("unable to create directory", x, 0);
    if (chdir(x) == -1) die_fatal("unable to chdir to directory", x, 0);

    for (i = 0; sshcrypto_keys[i].name; ++i) {
        if (sshcrypto_keys[i].sign_keypair(pk, sk) != 0) die_fatal("unable to generate key pair", x, 0);
        umask(022);
        create(x, sshcrypto_keys[i].sign_publickeyfilename, pk, sshcrypto_keys[i].sign_publickeybytes);
        umask(077);
        create(x, sshcrypto_keys[i].sign_secretkeyfilename, sk, sshcrypto_keys[i].sign_secretkeybytes);
    }
    global_die(0); return 111;
}
