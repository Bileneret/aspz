#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#define USER_FILE   "userfile.txt"
#define COPY_PATH   "/home/bileneret/Documents/copy.txt"

int main(void) {
    // Отримання реального та ефективного UID
    uid_t ruid = getuid();
    uid_t euid = geteuid();
    printf("Реальний UID: %d, Ефективний UID: %d\n", ruid, euid);

    // Створення файлу від імені звичайного користувача
    seteuid(ruid);
    int fd = open(USER_FILE, O_CREAT | O_TRUNC | O_WRONLY, 0644);
    if (fd < 0) {
        printf("Помилка створення %s: %s\n", USER_FILE, strerror(errno));
        return EXIT_FAILURE;
    }
    dprintf(fd, "Hello from user %d\n", ruid);
    close(fd);
    printf("Файл %s створено від імені користувача\n", USER_FILE);

    // Копіювання файлу з правами root
    seteuid(euid);
    int in = open(USER_FILE, O_RDONLY);
    if (in < 0) {
        printf("Помилка відкриття %s: %s\n", USER_FILE, strerror(errno));
        return EXIT_FAILURE;
    }
    int out = open(COPY_PATH, O_CREAT | O_TRUNC | O_WRONLY, 0644);
    if (out < 0) {
        printf("Помилка створення %s: %s\n", COPY_PATH, strerror(errno));
        close(in);
        return EXIT_FAILURE;
    }
    char buf[4096];
    ssize_t n;
    while ((n = read(in, buf, sizeof(buf))) > 0) {
        if (write(out, buf, n) < 0) {
            printf("Помилка запису в %s: %s\n", COPY_PATH, strerror(errno));
            break;
        }
    }
    close(in);
    close(out);
    printf("Файл скопійовано в %s від імені root\n", COPY_PATH);

    // Спроба додавання даних від імені звичайного користувача
    seteuid(ruid);
    int cw = open(COPY_PATH, O_WRONLY | O_APPEND);
    if (cw < 0) {
        printf("Помилка відкриття %s для запису: %s\n", COPY_PATH, strerror(errno));
    } else {
        if (write(cw, "Trying to append\n", 17) < 0) {
            printf("Помилка запису в %s: %s\n", COPY_PATH, strerror(errno));
        } else {
            printf("Додано дані до %s\n", COPY_PATH);
        }
        close(cw);
    }

    // Спроба видалення файлу
    if (unlink(COPY_PATH) == -1) {
        printf("Помилка видалення %s: %s\n", COPY_PATH, strerror(errno));
    } else {
        printf("Файл %s успішно видалено\n", COPY_PATH);
    }

    return 0;
}
