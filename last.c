#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#define DEFAULT_LINES 10
#define MAX_LINE_LENGTH 1024

// Структура для кільцевого буфера
typedef struct {
    char **lines;
    int size;
    int head;
    int count;
} RingBuffer;

// Ініціалізація кільцевого буфера
RingBuffer* init_buffer(int size) {
    RingBuffer *buffer = malloc(sizeof(RingBuffer));
    buffer->lines = malloc(size * sizeof(char*));
    for (int i = 0; i < size; i++) {
        buffer->lines[i] = malloc(MAX_LINE_LENGTH * sizeof(char));
    }
    buffer->size = size;
    buffer->head = 0;
    buffer->count = 0;
    return buffer;
}

// Додавання рядка до буфера
void add_line(RingBuffer *buffer, const char *line) {
    strncpy(buffer->lines[buffer->head], line, MAX_LINE_LENGTH - 1);
    buffer->lines[buffer->head][MAX_LINE_LENGTH - 1] = '\0';
    buffer->head = (buffer->head + 1) % buffer->size;
    if (buffer->count < buffer->size) {
        buffer->count++;
    }
}

// Вивільнення пам'яті буфера
void free_buffer(RingBuffer *buffer) {
    for (int i = 0; i < buffer->size; i++) {
        free(buffer->lines[i]);
    }
    free(buffer->lines);
    free(buffer);
}

// Виведення рядків із буфера
void print_buffer(RingBuffer *buffer, int reverse) {
    int start, step, end;
    if (reverse) {
        // Починаємо з останнього доданого рядка і рухаємося назад
        start = (buffer->head - 1 + buffer->size) % buffer->size;
        step = -1;
        end = (buffer->head - buffer->count + buffer->size) % buffer->size;
        for (int i = 0; i < buffer->count; i++) {
            printf("%s", buffer->lines[start]);
            start = (start + step + buffer->size) % buffer->size;
        }
    } else {
        // Починаємо з найстарішого рядка і рухаємося вперед
        start = (buffer->head - buffer->count + buffer->size) % buffer->size;
        step = 1;
        end = (buffer->head - 1 + buffer->size) % buffer->size;
        for (int i = 0; i < buffer->count; i++) {
            printf("%s", buffer->lines[start]);
            start = (start + step + buffer->size) % buffer->size;
        }
    }
}

int main(int argc, char *argv[]) {
    int n = DEFAULT_LINES;
    int reverse = 0;
    int opt;

    // Обробка аргументів командного рядка
    while ((opt = getopt(argc, argv, "n:r")) != -1) {
        switch (opt) {
            case 'n':
                n = atoi(optarg);
                if (n <= 0) {
                    fprintf(stderr, "Error: Number of lines must be positive\n");
                    return 1;
                }
                break;
            case 'r':
                reverse = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-n num] [-r] [file]\n", argv[0]);
                return 1;
        }
    }

    // Ініціалізація буфера
    RingBuffer *buffer = init_buffer(n);

    // Визначення джерела вводу
    FILE *input = stdin;
    if (optind < argc) {
        input = fopen(argv[optind], "r");
        if (!input) {
            perror("Error opening file");
            free_buffer(buffer);
            return 1;
        }
    }

    // Читання рядків
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, input)) {
        add_line(buffer, line);
    }

    // Закриття файлу, якщо не стандартний ввід
    if (input != stdin) {
        fclose(input);
    }

    // Виведення рядків
    print_buffer(buffer, reverse);

    // Вивільнення пам'яті
    free_buffer(buffer);
    return 0;
}
