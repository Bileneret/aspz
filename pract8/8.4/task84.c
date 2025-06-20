#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
    pid_t pid;
    pid = fork();
    printf("%d\n", pid);
    return 0;
}
