#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

void check_file_permission(const char *filename, const char *mode) {
    FILE *file = fopen(filename, mode);
    if (!file) {
        if (mode[0] == 'r') {
            perror("Cannot open file for reading");
        } else {
            perror("Cannot open file for writing");
        }
        exit(1);
    }
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Program need two arguments\n");
        return 1;
    }

    check_file_permission(argv[1], "r");
    check_file_permission(argv[2], "w");

    FILE *src = fopen(argv[1], "r");
    FILE *dest = fopen(argv[2], "w");

    char buffer[1024];
    size_t bytes;

    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, bytes, dest);
    }

    fclose(src);
    fclose(dest);
    return 0;
}