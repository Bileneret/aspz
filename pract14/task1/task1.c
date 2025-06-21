#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void handler(int sig) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char buf[64];
    strftime(buf, sizeof(buf), "%H:%M:%S\n", tm_info);
    write(STDOUT_FILENO, buf, strlen(buf));
    alarm(1);
}

int main(void) {
    signal(SIGALRM, handler);
    alarm(1);
    while (1) pause();
    return 0;
}
