#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 3) return 1;
    char buf[4096];
    FILE *f = fopen(argv[2], "r");
    while (fgets(buf, sizeof(buf), f))
        if (strstr(buf, argv[1]))
            fputs(buf, stdout);
    return 0;
}