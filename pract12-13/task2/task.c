#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void handler(int sig) {
    printf("\nGot SIGINT! (handler called)\n");
    fflush(stdout);
}

int main() {
    // Одразу повідомляємо про старт
    printf("Blocking SIGINT for 5 seconds. Try Ctrl+C now...\n");
    fflush(stdout);

    // Встановлюємо обробник
    signal(SIGINT, handler);

    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);

    // Блокуємо SIGINT
    sigprocmask(SIG_BLOCK, &set, NULL);

    sleep(5);  // тут ви нічого не бачите — сигнал блокується

    // Розблокування
    printf("Now unblocking SIGINT.\n");
    fflush(stdout);
    sigprocmask(SIG_UNBLOCK, &set, NULL);

    // Даємо час обробнику відпрацювати
    sleep(2);

    printf("Exiting normally.\n");
    return 0;
}
