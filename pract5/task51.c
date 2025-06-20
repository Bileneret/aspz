#include <stdio.h>
#include <stdlib.h>

// Глобальний вказівник для передачі між функціями
int *global_ptr = NULL;

void free_pointer() {
    printf("Функція free_pointer: Звільнення пам’яті за адресою %p\n", (void *)global_ptr);
    free(global_ptr); // Звільнення пам’яті
    global_ptr = NULL; // Хороша практика, але інша функція може не знати
}

void use_pointer() {
    if (global_ptr != NULL) {
        printf("Функція use_pointer: Значення за адресою %p = %d\n", (void *)global_ptr, *global_ptr);
    } else {
        printf("Функція use_pointer: Вказівник є NULL, доступ заборонено\n");
    }
}

int main() {
    // Виділення пам’яті
    global_ptr = (int *)malloc(sizeof(int));
    if (global_ptr == NULL) {
        perror("Помилка виділення пам’яті");
        return 1;
    }
    *global_ptr = 42; // Ініціалізація значення
    printf("main: Початкове значення за адресою %p = %d\n", (void *)global_ptr, *global_ptr);

    // Виклик функції, яка використовує вказівник
    use_pointer();

    // "Викрадення" — функція звільняє пам’ять
    free_pointer();

    // Спроба використання вказівника після звільнення
    use_pointer();

    return 0;
}
