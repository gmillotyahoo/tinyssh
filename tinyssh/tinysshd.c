/*
20181206
Jan Mojzis
Public domain.
*/

/*
Multi-call binary wrapper
*/

#include <unistd.h>
#include "str.h"
#include "main.h"

static char *basename(char *str) {

    char *s;
    char *ret = str;

    for (s = str; *s; ++s) {
        if (*s == '/') ret = s + 1;
    }
    return ret;
}

static char *x;
static long long xlen;

int main(int argc, char **argv) {

    if (argc < 1) _exit(100);
    if (!argv[0]) _exit(100);

    x = basename(argv[0]);
    if (!x) _exit(100);
    xlen = str_len(x);

    if (str_equaln(x, xlen, "tinysshd-printkey")) {
        return main_tinysshd_printkey(argc, argv, "tinysshd-printkey");
    }
    else if (str_equaln(x, xlen, "tinysshd-makekey")){
        return main_tinysshd_makekey(argc, argv, "tinysshd-makekey");
    }
    else if (str_equaln(x, xlen, "tinysshtunneld")){
        return main_tinysshd(argc, argv, "tinysshtunneld");
    }
    else {
        return main_tinysshd(argc, argv, "tinysshd");
    }

    _exit(111);
    return 111;
}
