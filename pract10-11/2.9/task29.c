#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>    // для exit()
#include <sys/wait.h>  // для wait()


int main() {
    int status;
    pid_t pid = fork();

    if (pid == 0) {
        // дочірній процес просто завершується
        exit(7);
    } else {
        pid_t w = wait(&status);
        if (WIFEXITED(status)) {
            printf("Child %d exited with code %d\n", w, WEXITSTATUS(status));
        }
    }
    return 0;
}
