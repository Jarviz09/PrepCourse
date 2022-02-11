#include "utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t custom_strlen(const char* str) {
    return strlen(str);
}

int custom_pow(int base, int power) {
    int result = 1;

    for (int i = 0; i < power; ++i) {
        result *= base;
    }

    return result;
}

bool is_converted(const char *end) {
    if (!end) {
        return false;
    }

    return *end == '\0';
}
