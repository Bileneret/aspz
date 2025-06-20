#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]){
    double n = (argc > 1) ? atof(argv[1]) : 1.0;
    srand((unsigned)time(NULL));

    printf("Random in [0.0,1.0]:\n");
    for(int i = 0; i < 10; i++)
        printf("%f\n", rand() / (RAND_MAX + 1.0));

    printf("\nRandom in [0.0,%.2f]:\n", n);
    for(int i = 0; i < 10; i++)
        printf("%f\n", (rand() / (RAND_MAX + 1.0)) * n);

    return 0;
}
