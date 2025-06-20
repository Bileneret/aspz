#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <limits.h>

void listdir(const char *dir) {
    DIR *d = opendir(dir);
    struct dirent *e;
    struct stat st;
    char path[PATH_MAX];

    if (!d) return;

    while ((e = readdir(d))) {
        if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0)
            continue;

        snprintf(path, sizeof(path), "%s/%s", dir, e->d_name);
        if (stat(path, &st) != 0)
            continue;

        if (S_ISDIR(st.st_mode)) {
            listdir(path);
        } else {
            printf("%s\n", path);
        }
    }

    closedir(d);
}

int main(void) {
    listdir(".");
    return 0;
}
