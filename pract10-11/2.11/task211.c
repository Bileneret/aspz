#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
int main() {
    pid_t pid = fork();
    int status;

    if (pid == 0) {
        // дочірній
        sleep(2);
        exit(0);
    } else {
        // чекаємо будь-якого
        wait(NULL);

        // аналогічно:
        waitpid(-1, &status, 0);

        // чекаємо конкретного PID
        waitpid(pid, &status, 0);

        // неблокуючий режим
        waitpid(pid, &status, WNOHANG);
    }
    return 0;
}
