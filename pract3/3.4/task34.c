#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>
#include <signal.h>
#include <unistd.h>

#define NUM_49 7
#define NUM_36 6

void handle_sigxcpu(int sig) {
    printf("\n[!] Час ЦП вичерпано. Програма завершила роботу.\n");
    exit(1);
}

void generate_lottery(int *arr, int count, int max) {
    int used[max + 1];
    for (int i = 0; i <= max; i++) used[i] = 0;

    for (int i = 0; i < count; ) {
        int n = rand() % max + 1;
        if (!used[n]) {
            arr[i++] = n;
            used[n] = 1;
        }
    }
}

int main() {
    signal(SIGXCPU, handle_sigxcpu);

    struct rlimit limit = {1, 2};
    setrlimit(RLIMIT_CPU, &limit);

    srand(time(NULL));

    int arr_49[NUM_49], arr_36[NUM_36];
    generate_lottery(arr_49, NUM_49, 49);
    generate_lottery(arr_36, NUM_36, 36);

    printf("Випало 7 з 49: ");
    for (int i = 0; i < NUM_49; i++) printf("%d ", arr_49[i]);

    printf("\nВипало 6 з 36: ");
    for (int i = 0; i < NUM_36; i++) printf("%d ", arr_36[i]);

    printf("\n");

    while (1) {
        
    }
    printf("не вдалось (");

    return 0;
}