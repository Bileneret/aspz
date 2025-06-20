#include <stdio.h>
#include <stdlib.h>

int main() {
    size_t size = 100;

    void *ptr = realloc(NULL, size);
    if (ptr) {
        printf("realloc(NULL, %zu) = malloc(%zu)\n", size, size);
        free(ptr);
    }

    ptr = malloc(size);
    void *newptr = realloc(ptr, 0);
    if (newptr == NULL) {
        printf("realloc(ptr, 0) повернув NULL (пам’ять звільнена)\n");
    }

    return 0;
}