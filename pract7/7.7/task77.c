/* grant_read.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void) {
    DIR *d = opendir(".");
    struct dirent *e;
    struct stat st;

    if (!d) return 1;
    while ((e = readdir(d))) {
        char *name = e->d_name;
        size_t len = strlen(name);
        if (len > 2 && strcmp(name + len - 2, ".c") == 0
            && stat(name, &st) == 0 && S_ISREG(st.st_mode)) {

            printf("Grant read permission to others for \"%s\"? (y/n): ", name);
            int c = getchar();
            while (getchar() != '\n');
            if (c == 'y' || c == 'Y') {
                if (chmod(name, st.st_mode | S_IROTH) == 0)
                    printf("Permission updated for %s.\n\n", name);
                else
                    perror("chmod error");
            } else {
                printf("Skipped %s.\n\n", name);
            }
        }
    }
    closedir(d);
    return 0;
}
