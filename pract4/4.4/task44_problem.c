#include <stdio.h>
#include <stdlib.h>

int main() {
    void *ptr = NULL;
    int i = 0;

    while (i++ < 5) {
        if (!ptr)
            ptr = malloc(10);

        ((char*)ptr)[0] = 'A';
        printf("Iteration %d: %c\n", i, ((char*)ptr)[0]);

        free(ptr); 
    }

    return 0;
}