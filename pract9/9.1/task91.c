#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define UID_MIN 1000 // Мінімальний UID для звичайних користувачів в Ubuntu

int main(void) {
    // Отримання UID поточного користувача
    uid_t current_uid = getuid();
    printf("Поточний користувач UID: %d\n", current_uid);

    // Відкриття потоку для виконання команди getent passwd
    FILE *fp = popen("getent passwd", "r");
    if (!fp) {
        perror("Помилка виконання getent passwd");
        exit(EXIT_FAILURE);
    }
    printf("Команда getent passwd успішно запущена\n");

    char *line = NULL;
    size_t len = 0;
    int found = 0;

    // Читання виводу команди по рядках
    while (getline(&line, &len, fp) != -1) {
        // Створення копії рядка для безпечної обробки
        char *copy = strdup(line);
        if (!copy) {
            perror("Помилка виділення пам'яті");
            free(line);
            pclose(fp);
            exit(EXIT_FAILURE);
        }

        // Розбиття рядка на поля
        char *user = strtok(copy, ":");
        if (!user) {
            free(copy);
            continue;
        }
        strtok(NULL, ":"); // Пропускаємо пароль
        char *uid_str = strtok(NULL, ":");
        if (!uid_str) {
            free(copy);
            continue;
        }

        // Перетворення UID у число
        char *endptr;
        long uid = strtol(uid_str, &endptr, 10);
        if (*endptr != '\0' && *endptr != '\n') {
            free(copy);
            continue;
        }

        // Перевірка, чи є користувач звичайним і не є поточним
        if (uid >= UID_MIN && (uid_t)uid != current_uid) {
            printf("Знайдено іншого користувача: %s (UID=%ld)\n", user, uid);
            found = 1;
        }

        free(copy);
    }

    // Очищення та закриття
    free(line);
    if (pclose(fp) == -1) {
        perror("Помилка закриття потоку getent");
        exit(EXIT_FAILURE);
    }

    // Виведення результату, якщо інших користувачів не знайдено
    if (!found) {
        printf("Інших звичайних користувачів не знайдено\n");
    }

    return 0;
}
