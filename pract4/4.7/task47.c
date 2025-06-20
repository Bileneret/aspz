#include <stdio.h>
#include <stdlib.h>

struct sbar {
    int value;
};

int main() {
    struct sbar *buffer = NULL;
    struct sbar *resized = NULL;

    buffer = calloc(1000, sizeof(struct sbar));
    if (!buffer) {
        perror("Initial calloc failed");
        return 1;
    }

    printf("Initial memory allocated: buffer = %p\n", buffer);

    resized = reallocarray(buffer, 500, sizeof(struct sbar));
    if (!resized) {
        perror("reallocarray failed");
        free(buffer);
        return 1;
    }

    printf("Reallocarray completed: resized = %p\n", resized);

    free(resized);
    return 0;
}
