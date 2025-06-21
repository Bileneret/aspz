#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <sys/select.h>
#include <errno.h> // для errno та EINTR

volatile sig_atomic_t is_active = 0;
struct timespec begin_time, finish_time;

void signal_handler(int sig) {
    clock_gettime(CLOCK_MONOTONIC, &begin_time);
    printf("\nСТАРТ!\n");
    fflush(stdout);
    is_active = 1;
}

int main(void) {
    signal(SIGALRM, signal_handler);
    srand(time(NULL));

    while (1) {
        int wait_time = (rand() % 4) * 1000000 + 2000000;  // 2–5 сек у мікросекундах

        printf("Готуйтесь... чекайте на сигнал СТАРТ\n");
        fflush(stdout);
        is_active = 0;
        alarm(wait_time / 1000000);  // встановлюємо сигнал через X секунд

        fd_set readfds;
        struct timeval tv;
        int retval;

        while (!is_active) {
            FD_ZERO(&readfds);
            FD_SET(STDIN_FILENO, &readfds);
            tv.tv_sec = 0;
            tv.tv_usec = 100000;  // перевірка кожні 0.1 сек

            retval = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);
            if (retval == -1) {
                if (errno == EINTR) {
                    continue;  // select перерваний сигналом — нічого страшного
                } else {
                    perror("select");
                    exit(EXIT_FAILURE);
                }
            } else if (retval > 0) {
                if (!is_active) {
                    printf("Хитрун?\n");
                    fflush(stdout);
                    // очистити буфер вводу
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF);
                    // перейти до наступної спроби
                    goto next_round;
                }
            }
        }

        // після "СТАРТ!" — чекаємо на реакцію користувача
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        if (select(STDIN_FILENO + 1, &readfds, NULL, NULL, NULL) == -1) {
            perror("select після СТАРТ");
            exit(EXIT_FAILURE);
        }

        // зчитуємо натискання
        getchar();
        clock_gettime(CLOCK_MONOTONIC, &finish_time);

        long diff_ms = (finish_time.tv_sec - begin_time.tv_sec) * 1000 +
                       (finish_time.tv_nsec - begin_time.tv_nsec) / 1000000;

        printf("Час реакції: %ld мс\n\n", diff_ms);
        printf("Натисніть Enter для наступної спроби або Ctrl+C для виходу...\n");

        getchar(); // чекаємо перед наступним запуском

    next_round:
        continue;
    }

    return 0;
}

