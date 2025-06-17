#include <stdio.h>
#include <stdlib.h>

void recursive_function(int depth) {
    char buffer[1024]; 
    printf("Рекурсія глибина: %d\n", depth);
    if (depth > 0) {
        recursive_function(depth - 1);
    }
}

int main() {
    recursive_function(10000); 
    return 0;
}