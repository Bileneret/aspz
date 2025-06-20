#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>

int cmpstr(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int main(void) {
    DIR *d = opendir(".");
    struct dirent *e;
    struct stat st;
    char **names = NULL;
    size_t count = 0, cap = 0;
    char path[PATH_MAX];

    if (!d) {
        perror("opendir");
        return 1;
    }

    while ((e = readdir(d))) {
        if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0)
            continue;
        snprintf(path, sizeof(path), "./%s", e->d_name);
        if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
            if (count == cap) {
                cap = cap ? cap * 2 : 8;
                names = realloc(names, cap * sizeof(*names));
                if (!names) {
                    perror("realloc");
                    closedir(d);
                    return 1;
                }
            }
            names[count++] = strdup(e->d_name);
        }
    }
    closedir(d);

    if (count) {
        qsort(names, count, sizeof(*names), cmpstr);
        for (size_t i = 0; i < count; i++) {
            printf("%s\n", names[i]);
            free(names[i]);
        }
        free(names);
    }

    return 0;
}
