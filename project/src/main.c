#include "prime.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

#define ERR_ARGS_COUNT (-1)
#define ERR_WRONG_FLG (-2)
#define ERR_WRONG_ARG (-3)

#define TST_FOO_FIX (1)
#define TST_FOO_IMPL (2)
#define TST_MOD_IMPL (3)

int main(int argc, const char** argv) {
    if (argc < 3) {
        return ERR_ARGS_COUNT;
    }

    char *end = NULL;

    int test_case = strtol(argv[1], &end, 10);

    if (!is_converted(end)) {
        return ERR_WRONG_ARG;
    }

    const char* data;
    data = argv[2];
    switch (test_case) {
        case TST_FOO_FIX: {
            printf("%zu\n", custom_strlen(data));
            break;
        }
        case TST_FOO_IMPL: {
            if (argc != 4) {
                return ERR_ARGS_COUNT;
            }

            int base = strtol(data, &end, 10);

            if (!is_converted(end)) {
                return ERR_WRONG_ARG;
            }

            int pow = strtol(argv[3], &end, 10);

            if (!is_converted(end)) {
                return ERR_WRONG_ARG;
            }

            printf("%d\n", custom_pow(base, pow));
            break;
        }
        case TST_MOD_IMPL: {
            int num = strtol(data, &end, 10);

            if (!is_converted(end)) {
                return ERR_WRONG_ARG;
            }

            printf("%d\n", is_prime(num));

            break;
        }
        default: {
            return ERR_WRONG_FLG;
        }
    }

    return 0;
}
