#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

int main(void) {
    DIR *d = opendir(".");
    struct dirent *e;
    struct stat st;

    if (!d) {
        perror("opendir");
        return 1;
    }

    while ((e = readdir(d))) {
        if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0)
            continue;
        if (stat(e->d_name, &st) == 0 && S_ISREG(st.st_mode)) {
            printf("Delete file \"%s\"? (y/n): ", e->d_name);
            int c = getchar();
            while (getchar() != '\n');
            if (c == 'y' || c == 'Y') {
                if (unlink(e->d_name) == 0)
                    printf("%s deleted.\n\n", e->d_name);
                else
                    perror("unlink error");
            } else {
                printf("Skipped %s.\n\n", e->d_name);
            }
        }
    }

    closedir(d);
    return 0;
}
