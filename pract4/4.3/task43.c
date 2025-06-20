#include <stdio.h>
#include <stdlib.h>

int main() {
    void *memory = malloc(0);

    if (memory == NULL) {
        printf("malloc(0) повернув NULL\n");
    } else {
        printf("malloc(0) повернув непорожній вказівник: %p\n", memory);
        free(memory);
    }

    return 0;
}
