#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <PID>\n", argv[0]);
        return 1;
    }
    pid_t pid = atoi(argv[1]);

    union sigval val;
    val.sival_int = 99;

    if (sigqueue(pid, SIGRTMIN, val) == -1) {
        perror("sigqueue failed");
    } else {
        printf("Sent SIGRTMIN to PID %d with value %d\n", pid, val.sival_int);
    }

    printf("Press Enter to exit...\n");
    getchar();
    return 0;
}
