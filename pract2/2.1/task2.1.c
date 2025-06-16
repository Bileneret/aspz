#include <stdio.h>
#include <time.h>

int main() {
    time_t time;
    size_t time_size = sizeof(time);

    printf("Тип time_t = %zu байт\n", time_size);

    if (time_size == 4) {
        time_t limit_time = 0x7FFFFFFF;
        printf("На 32-бітній архітектурі час досягне межі: %s", ctime(&limit_time));
    } else {
        printf("На 64-бітних системах обмеження часу недосяжне.");
    }

    return 0;
}

