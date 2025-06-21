#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void handler(int sig, siginfo_t *info, void *ctx) {
    printf("Caught signal %d from PID %d\n", sig, info->si_pid);
}

int main() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = handler;
    sa.sa_flags = SA_SIGINFO | SA_RESTART;
    sigaction(SIGUSR1, &sa, NULL);
    printf("PID: %d\n", getpid());
    while (1) pause();
}
