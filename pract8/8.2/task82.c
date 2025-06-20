#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
    unsigned char data[] = {4, 5, 2, 2, 3, 3, 7, 9, 1, 5};
    size_t data_size = sizeof(data);
    if (write(fd, data, data_size) != (ssize_t)data_size) {
        perror("Помилка запису даних у файл");
        close(fd);
        exit(EXIT_FAILURE);
    }
    printf("Записано %zu байтів: ", data_size);
    for (size_t i = 0; i < data_size; i++) {
        printf("%u ", data[i]);
    }
    printf("\n");

    // Переміщення покажчика на позицію 3
    if (lseek(fd, 3, SEEK_SET) == -1) {
        perror("Помилка переміщення покажчика");
        close(fd);
        exit(EXIT_FAILURE);
    }
    printf("Покажчик переміщено на позицію 3\n");

    // Читання 4 байтів у буфер
    unsigned char buffer[4];
    memset(buffer, 0, sizeof(buffer)); // Ініціалізація буфера нулями
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer));
    if (bytes_read < 0) {
        perror("Помилка читання з файлу");
        close(fd);
        exit(EXIT_FAILURE);
    }
    printf("Прочитано %zd байтів\n", bytes_read);

    // Виведення вмісту буфера
    printf("Вміст буфера: %u %u %u %u\n",
           buffer[0], buffer[1], buffer[2], buffer[3]);

    // Закриття файлу
    close(fd);
    printf("Файл закрито\n");

    return 0;
}
