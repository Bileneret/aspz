#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define TEMPLATE "/tmp/tmpfileXXXXXX"

int main(void) {
    // Отримання реального та ефективного UID
    uid_t ruid = getuid();
    uid_t euid = geteuid();
    printf("Реальний UID: %d, Ефективний UID: %d\n", ruid, euid);

    // Створення тимчасового файлу від імені звичайного користувача
    char tmpl[] = TEMPLATE;
    int fd = mkstemp(tmpl);
    if (fd < 0) {
        printf("Помилка створення тимчасового файлу: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    if (write(fd, "test\n", 5) < 0) {
        printf("Помилка запису в тимчасовий файл: %s\n", strerror(errno));
        close(fd);
        unlink(tmpl);
        return EXIT_FAILURE;
    }
    close(fd);
    printf("Тимчасовий файл %s створено від імені користувача\n", tmpl);

    // Зміна власника та прав доступу від імені root
    seteuid(euid);
    if (chown(tmpl, ruid, ruid) == -1) {
        printf("Помилка chown: %s\n", strerror(errno));
        seteuid(ruid);
        unlink(tmpl);
        return EXIT_FAILURE;
    }
    if (chmod(tmpl, S_IRUSR | S_IWUSR) == -1) {
        printf("Помилка chmod: %s\n", strerror(errno));
        seteuid(ruid);
        unlink(tmpl);
        return EXIT_FAILURE;
    }
    printf("Права та власник файлу змінено від імені root\n");

    // Перевірка доступу від імені звичайного користувача
    seteuid(ruid);
    int r = open(tmpl, O_RDONLY);
    if (r < 0) {
        printf("Читання файлу: Помилка - %s\n", strerror(errno));
    } else {
        printf("Читання файлу: OK\n");
        close(r);
    }

    int w = open(tmpl, O_WRONLY | O_APPEND);
    if (w < 0) {
        printf("Запис у файл: Помилка - %s\n", strerror(errno));
    } else {
        printf("Запис у файл: OK\n");
        close(w);
    }

    // Видалення тимчасового файлу
    if (unlink(tmpl) == -1) {
        printf("Помилка видалення файлу: %s\n", strerror(errno));
    } else {
        printf("Тимчасовий файл видалено\n");
    }

    return 0;
}
