#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main() {
    size_t size_type_bytes = sizeof(size_t);
    size_t max_alloc_value = (size_t)(-1);

    printf("Розмір типу size_t: %zu байт\n", size_type_bytes);
    printf("Найбільше значення, яке можна передати malloc: %zu\n", max_alloc_value);

    return 0;
}
