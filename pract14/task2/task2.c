#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

timer_t timerid;

void handler(int sig) {
    write(STDOUT_FILENO, "Tic\n", 5);
}

int main() {
    signal(SIGRTMIN, handler);
    struct sigevent sev = {0};
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo  = SIGRTMIN;
    timer_create(CLOCK_REALTIME, &sev, &timerid);
    struct itimerspec its = {{1, 0}, {1, 0}};
    timer_settime(timerid, 0, &its, NULL);
    while (1) pause();
    return 0;
}
