// no_zombie_demo.c
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    // Налаштовуємо: ігнорувати SIGCHLD і автоматично збирати дочок
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = SIG_IGN;
    sa.sa_flags   = SA_NOCLDWAIT;
    sigaction(SIGCHLD, &sa, NULL);

    printf("== BEFORE fork: showing zombies ==\n");
    system("ps -l | grep Z");  // шукаємо процеси в стані Z

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }
    if (pid == 0) {
        // дитина просто завершується одразу
        _exit(0);
    }

    // даємо час ОС «забарвити» дитину в зомбі, якби вона не була SA_NOCLDWAIT
    sleep(1);

    printf("== AFTER fork and sleep: showing zombies ==\n");
    system("ps -l | grep Z");

    printf("If SA_NOCLDWAIT works, ви не побачите рядків із 'Z'.\n");
    return 0;
}
