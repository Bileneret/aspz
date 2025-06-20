#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

// Функція порівняння для цілих чисел
int compare_ints(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return (ia > ib) - (ia < ib);
}

// Генерація зростаючого масиву
void generate_increasing(int *arr, size_t n) {
    for (size_t i = 0; i < n; i++) {
        arr[i] = (int)i;
    }
}

// Генерація спадного масиву
void generate_decreasing(int *arr, size_t n) {
    for (size_t i = 0; i < n; i++) {
        arr[i] = (int)(n - i - 1);
    }
}

// Генерація константного масиву
void generate_constant(int *arr, size_t n) {
    for (size_t i = 0; i < n; i++) {
        arr[i] = 42;
    }
}

// Генерація випадкового масиву
void generate_random(int *arr, size_t n) {
    for (size_t i = 0; i < n; i++) {
        arr[i] = rand() % 1000000;
    }
}

// Генерація майже відсортованого масиву (з невеликими змінами)
void generate_almost_sorted(int *arr, size_t n) {
    generate_increasing(arr, n);
    for (size_t i = 0; i < n / 100; i++) { // Змінюємо 1% елементів
        size_t idx1 = rand() % n;
        size_t idx2 = rand() % n;
        int temp = arr[idx1];
        arr[idx1] = arr[idx2];
        arr[idx2] = temp;
    }
}

// Перевірка, чи масив відсортований
int is_sorted(int *arr, size_t n) {
    for (size_t i = 1; i < n; i++) {
        if (arr[i - 1] > arr[i]) {
            return 0;
        }
    }
    return 1;
}

// Вимірювання часу виконання qsort (в мікросекундах)
long long timed_qsort(int *arr, size_t n) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    qsort(arr, n, sizeof(int), compare_ints);
    clock_gettime(CLOCK_MONOTONIC, &end);
    return (end.tv_sec - start.tv_sec) * 1000000LL + (end.tv_nsec - start.tv_nsec) / 1000;
}

// Тестування коректності qsort
void run_tests() {
    printf("\n=== Початок тестів коректності qsort ===\n");

    // Тест 1: Порожній масив
    int empty[] = {};
    qsort(empty, 0, sizeof(int), compare_ints);
    printf("Тест 1 (порожній масив): %s\n", is_sorted(empty, 0) ? "Пройдено" : "Провалено");

    // Тест 2: Масив з одним елементом
    int single[] = {42};
    qsort(single, 1, sizeof(int), compare_ints);
    printf("Тест 2 (один елемент): %s\n", is_sorted(single, 1) ? "Пройдено" : "Провалено");

    // Тест 3: Випадковий масив
    int random[] = {5, 2, 9, 1, 5, 6};
    qsort(random, 6, sizeof(int), compare_ints);
    printf("Тест 3 (випадковий масив): %s\n", is_sorted(random, 6) ? "Пройдено" : "Провалено");

    // Тест 4: Уже відсортований масив
    int sorted[] = {1, 2, 3, 4, 5};
    qsort(sorted, 5, sizeof(int), compare_ints);
    printf("Тест 4 (відсортований масив): %s\n", is_sorted(sorted, 5) ? "Пройдено" : "Провалено");

    // Тест 5: Масив із дублікатами
    int duplicates[] = {3, 3, 3, 2, 2, 1};
    qsort(duplicates, 6, sizeof(int), compare_ints);
    printf("Тест 5 (дублікати): %s\n", is_sorted(duplicates, 6) ? "Пройдено" : "Провалено");

    printf("=== Кінець тестів ===\n\n");
}

int main(void) {
    // Ініціалізація генератора випадкових чисел
    srand(time(NULL));

    // Запуск тестів коректності
    run_tests();

    // Налаштування експерименту
    size_t sizes[] = {10000, 50000, 100000, 200000};
    const char *names[] = {"increasing", "decreasing", "constant", "random", "almost_sorted"};
    void (*generators[])(int *, size_t) = {
        generate_increasing,
        generate_decreasing,
        generate_constant,
        generate_random,
        generate_almost_sorted
    };
    size_t num_types = sizeof(generators) / sizeof(generators[0]);

    // Заголовок результатів
    printf("Розмір,  Тип масиву,  Час (мкс),  Коректність\n");

    // Основний цикл експерименту
    for (size_t i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++) {
        size_t n = sizes[i];
        int *arr = malloc(n * sizeof(int));
        int *copy = malloc(n * sizeof(int));
        if (!arr || !copy) {
            fprintf(stderr, "Помилка виділення пам'яті\n");
            exit(EXIT_FAILURE);
        }

        for (size_t t = 0; t < num_types; t++) {
            generators[t](arr, n);
            memcpy(copy, arr, n * sizeof(int)); // Збереження копії для сортування
            long long time_us = timed_qsort(copy, n);
            int correct = is_sorted(copy, n);
            printf("%zu,  %s,  %lld,  %d\n", n, names[t], time_us, correct);
        }

        free(arr);
        free(copy);
    }

    return 0;
}
