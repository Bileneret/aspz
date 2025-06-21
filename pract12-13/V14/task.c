#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

volatile sig_atomic_t processed = 0;
volatile sig_atomic_t dispatched = 0;
timer_t timer_handle;

void signal_processor(int sig) {
    int excess = timer_getoverrun(timer_handle);
    dispatched += 1 + (excess > 0 ? excess : 0);
    processed++;
    printf("Оброблено сигнал, загалом: %d\n", processed); // Відладка
    fflush(stdout);
}

int main(void) {
    if (signal(SIGRTMIN, signal_processor) == SIG_ERR) {
        perror("Помилка налаштування сигналу");
        exit(EXIT_FAILURE);
    }

    struct sigevent event = {0};
    event.sigev_notify = SIGEV_SIGNAL;
    event.sigev_signo = SIGRTMIN;
    event.sigev_value.sival_ptr = &timer_handle;

    if (timer_create(CLOCK_REALTIME, &event, &timer_handle) == -1) {
        perror("Помилка створення таймера");
        exit(EXIT_FAILURE);
    }

    struct itimerspec timer_spec = {{0, 10000000}, {0, 10000000}};
    if (timer_settime(timer_handle, 0, &timer_spec, NULL) == -1) {
        perror("Помилка встановлення таймера");
        exit(EXIT_FAILURE);
    }

    printf("Таймер запущено: сигнали кожні 1 мс протягом 2 секунд...\n");
    fflush(stdout);

    sleep(2);

    timer_spec.it_value.tv_sec = 0;
    timer_spec.it_value.tv_nsec = 0;
    timer_spec.it_interval = timer_spec.it_value;
    if (timer_settime(timer_handle, 0, &timer_spec, NULL) == -1) {
        perror("Помилка зупинки таймера");
        exit(EXIT_FAILURE);
    }

    sleep(1);

    printf("Відправлено (з перевантаженнями): %d\n", dispatched);
    printf("Виклики обробника:              %d\n", processed);
    printf("Загублені сигнали:              %d\n", dispatched - processed);

    timer_delete(timer_handle);
    return 0;
}
