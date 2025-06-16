#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define INFO_MSG "У функції %20s: адреса локальної змінної → %p\n"

static void inner_pause() {
    int temp = 123;
    printf(INFO_MSG, __func__, &temp);
    printf("\nОчікування завершення через pause()...\n");
    pause(); 
}

static void middle_layer() {
    int mid = 456;
    printf(INFO_MSG, __func__, &mid);
    inner_pause();
}

static void start_process() {
    int start = 789;
    printf(INFO_MSG, __func__, &start);
    middle_layer();
}

int main() {
    int main_var = 0;
    printf(INFO_MSG, __func__, &main_var);
    start_process();
    exit(EXIT_SUCCESS);
}

