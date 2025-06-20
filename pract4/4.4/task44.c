#include <stdio.h>
#include <stdlib.h>

int main() {
    void *ptr = malloc(10);
    if (!ptr) {
        perror("malloc failed");
        return 1;
    }

    int i = 0;
    while (i++ < 3) {
        ((char*)ptr)[0] = 'A' + i;
        printf("Iteration %d: %c\n", i, ((char*)ptr)[0]);
    }

    free(ptr);
    return 0;
}