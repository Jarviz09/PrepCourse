#include "prime.h"

#include <string.h>

int is_prime(int n) {
    int is_prime = 1;

    if ((n % 2 == 0 && n != 2) || (n < 2)) {
        return 0;
    }

    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            is_prime = 0;
            break;
        }
    }

    return is_prime;
}
