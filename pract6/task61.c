#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // Test Case #1: Неініціалізована змінна
    int uninit_var;
    printf("Uninitialized value: %d\n", uninit_var);

    // Test Case #5: Переповнення читання масиву
    char buffer[5] = "1234";
    char extra = buffer[5]; // Читання поза межами
    printf("Extra byte: %d\n", extra);

    // Test Case #8: Use-after-free
    int *ptr = (int *)malloc(sizeof(int));
    *ptr = 42;
    free(ptr);
    printf("Value after free: %d\n", *ptr); // Доступ після звільнення

    // Test Case #13: Витік пам’яті
    char *leak = strdup("Memory leak");
    // Немає free(leak)

    return 0;
}
