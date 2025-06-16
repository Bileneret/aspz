#include <stdio.h>

int global_value = 42;  

void use_stack() {
    int temp_array[1000]; 
    printf("Адреса масиву на стеку: %p\n", (void*)temp_array);
}

int main() {
    int local_var = 1; 

    printf("Ймовірна вершина стека: %p\n", (void*)&local_var);
    printf("Глобальна змінна (data segment): %p\n", (void*)&global_value);
    printf("Адреса функції (text segment): %p\n", (void*)use_stack);

    use_stack();

    return 0;
}

