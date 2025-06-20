#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    // Відкриття/створення файлу
    int fd = open("data.bin", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Помилка відкриття файлу");
        exit(EXIT_FAILURE);
    }
    printf("Файл 'data.bin' відкрито/створено\n");

    // Запис даних у файл
    const char *data = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    size_t data_len = strlen(data);
    if (write(fd, data, data_len) != (ssize_t)data_len) {
        perror("Помилка запису даних у файл");
        close(fd);
        exit(EXIT_FAILURE);
    }
    printf("Записано %zu байтів: %s\n", data_len, data);

    // Тестування pread з різними позиціями
    char buf[11] = {0}; // Буфер із запасом для нуль-термінатора
    off_t positions[] = {0, 5, 10, 15, 25}; // Позиції для читання
    size_t num_positions = sizeof(positions) / sizeof(positions[0]);
    printf("\nТестування pread з різними позиціями:\n");
    for (size_t i = 0; i < num_positions; i++) {
        ssize_t bytes_read = pread(fd, buf, 10, positions[i]);
        if (bytes_read < 0) {
            perror("Помилка pread");
            close(fd);
            exit(EXIT_FAILURE);
        }
        printf("pread з позиції %2ld: %.*s (%zd байтів)\n",
               (long)positions[i], (int)bytes_read, buf, bytes_read);
        memset(buf, 0, sizeof(buf)); // Очищення буфера
    }

    // Демонстрація незмінності покажчика файлу після pread
    printf("\nДемонстрація незмінності покажчика файлу:\n");
    off_t before = lseek(fd, 3, SEEK_SET);
    if (before == -1) {
        perror("Помилка lseek");
        close(fd);
        exit(EXIT_FAILURE);
    }
    printf("Покажчик файлу до pread: %ld\n", (long)before);

    if (pread(fd, buf, 4, 15) < 0) {
        perror("Помилка pread");
        close(fd);
        exit(EXIT_FAILURE);
    }
    printf("pread з позиції 15: %.4s\n", buf);

    off_t after = lseek(fd, 0, SEEK_CUR);
    if (after == -1) {
        perror("Помилка lseek");
        close(fd);
        exit(EXIT_FAILURE);
    }
    printf("Покажчик файлу після pread: %ld\n", (long)after);

    // Читання за допомогою read для порівняння
    memset(buf, 0, sizeof(buf));
    if (read(fd, buf, 4) != 4) {
        perror("Помилка read");
        close(fd);
        exit(EXIT_FAILURE);
    }
    printf("read після pread: %.4s\n", buf);

    // Закриття файлу
    if (close(fd) == -1) {
        perror("Помилка закриття файлу");
        exit(EXIT_FAILURE);
    }
    printf("Файл закрито\n");

    return 0;
}
