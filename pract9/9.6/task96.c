#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>

int try_open(const char *path, int flags) {
    int fd = open(path, flags);
    if (fd < 0) {
        printf("%s %s: %s\n",
               (flags & O_RDONLY) ? "Читання" : "Запис",
               path, strerror(errno));
    } else {
        printf("%s %s: OK\n",
               (flags & O_RDONLY) ? "Читання" : "Запис",
               path);
        close(fd);
    }
    return fd;
}

void try_exec(const char *path) {
    pid_t pid = fork();
    if (pid < 0) {
        printf("fork %s: %s\n", path, strerror(errno));
        return;
    }
    if (pid == 0) {
        execl(path, path, NULL);
        printf("exec %s: %s\n", path, strerror(errno));
        exit(1);
    } else {
        int status;
        if (waitpid(pid, &status, 0) > 0) {
            if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                printf("exec %s: OK\n", path);
            } else {
                printf("exec %s: failed (%d)\n", path, WEXITSTATUS(status));
            }
        }
    }
}

int main(void) {
    char pathbuf[PATH_MAX];
    char *home = getenv("HOME");

    // Виведення прав доступу за допомогою ls -l
    printf("Домашній каталог (~):\n");
    char cmd[PATH_MAX];
    snprintf(cmd, sizeof(cmd), "ls -l %s", home ? home : ".");
    system(cmd);

    printf("\n/usr/bin (зразок):\n");
    system("ls -l /usr/bin | head -n5");

    printf("\n/etc (зразок):\n");
    system("ls -l /etc | head -n5");

    // Визначення тестових файлів
    if (home) {
        snprintf(pathbuf, sizeof(pathbuf), "%s/.profile", home);
    } else {
        strcpy(pathbuf, "/tmp/.profile"); // Альтернатива, якщо HOME не визначено
    }
    const char *tests[] = {
        pathbuf,    // Файл у домашньому каталозі
        "/bin/ls",  // Виконуваний файл
        "/etc/shadow" // Обмежений файл
    };

    // Тестування доступу до файлів
    for (int i = 0; i < 3; i++) {
        const char *p = tests[i];
        printf("\n=== %s ===\n", p);
        try_open(p, O_RDONLY);
        try_open(p, O_WRONLY | O_APPEND);
        try_exec(p);
    }

    return 0;
}
