#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

int main() {
    int size_neg = -1;
    void *mem_neg = malloc(size_neg);
    if (mem_neg == NULL) {
        perror("malloc failed for negative size");
    } else {
        printf("malloc succeeded for negative size\n");
        free(mem_neg);
    }

    int val1 = INT_MAX;
    int val2 = 2;
    int result = val1 * val2;
    printf("val1: %d, val2: %d, result: %d\n", val1, val2, result);

    void *mem_overflow_int = malloc(result);
    if (mem_overflow_int == NULL) {
        perror("malloc failed on int overflow");
    } else {
        printf("malloc succeeded on int overflow\n");
        free(mem_overflow_int);
    }

    size_t s1 = SIZE_MAX / 2;
    size_t s2 = 3;
    size_t total = s1 * s2;
    printf("s1: %zu, s2: %zu, total: %zu\n", s1, s2, total);

    void *mem_overflow_size = malloc(total);
    if (mem_overflow_size == NULL) {
        perror("malloc failed on size_t overflow");
    } else {
        printf("malloc succeeded on size_t overflow\n");
        free(mem_overflow_size);
    }

    return 0;
}
