#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

int main() {
    int pipefd[2];
    char buffer[10000]; // Буфер розміром 10 КБ
    ssize_t count;
    const size_t nbytes = sizeof(buffer);

    // Ініціалізація буфера символами 'A'
    memset(buffer, 'A', nbytes);

    // Вивід інформації про буфер
    printf("Підготовка буфера: розмір %zu байтів\n", nbytes);
    printf("Перші 10 символів буфера: %.10s...\n", buffer);

    // Створення пайпа
    if (pipe(pipefd) == -1) {
        perror("Помилка створення пайпа");
        exit(EXIT_FAILURE);
    }
    printf("Пайп створено успішно\n");

    // Закриваємо кінець читання
    close(pipefd[0]);
    printf("Кінець читання пайпа закрито\n");

    // Налаштування вихідного дескриптора на неблокуючий режим
    if (fcntl(pipefd[1], F_SETFL, O_NONBLOCK) == -1) {
        perror("Помилка налаштування неблокуючого режиму");
        exit(EXIT_FAILURE);
    }
    printf("Неблокуючий режим увімкнено\n");

    // Спроба записати 10 КБ у пайп
    printf("Спроба запису %zu байтів у пайп...\n", nbytes);
    count = write(pipefd[1], buffer, nbytes);
    if (count == -1) {
        perror("Помилка запису");
    } else {
        printf("Запит на запис: %zu байтів, фактично записано: %zd байтів\n", nbytes, count);
        if (count < (ssize_t)nbytes) {
            printf("Записано менше даних, ніж очікувалося, через обмеження буфера пайпа\n");
        } else {
            printf("Усі дані успішно записано\n");
        }
    }

    // Закриваємо пайп
    close(pipefd[1]);
    printf("Пайп закрито\n");

    return 0;
}
