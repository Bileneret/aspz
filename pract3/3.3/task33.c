#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

#define MAX_LOG_BYTES 128
#define LOGFILE       "dice_log.txt"

static int is_over_limit(const char *path, size_t limit) {
    struct stat st;
    if (stat(path, &st) == 0)
        return (size_t)st.st_size > limit;
    return 0;
}

static int prompt_clear(void) {
    char buf[8];
    printf("Log file too big (%d bytes). Clear it and continue? [y/N]: ", MAX_LOG_BYTES);
    if (fgets(buf, sizeof(buf), stdin) == NULL) return 0;
    return (buf[0]=='y'||buf[0]=='Y');
}

int main(void) {
    FILE *logf;
    int face;
    srand((unsigned)time(NULL));

    logf = fopen(LOGFILE, "a");
    if (!logf) {
        perror("Cannot open log file");
        return 1;
    }

    puts("Dice roller active. ^C to quit.");

    while (1) {
        if (is_over_limit(LOGFILE, MAX_LOG_BYTES)) {
            if (prompt_clear()) {
                fclose(logf);
                logf = fopen(LOGFILE, "w");
                if (!logf) {
                    perror("Failed to reset log");
                    return 1;
                }
                puts("Log reset.");
            } else {
                puts("Stopping.");
                break;
            }
        }

        face = (rand() % 6) + 1;
        printf("Rolled: %d\n", face);
        fprintf(logf, "Rolled: %d\n", face);
        fflush(logf);

        usleep(100000);
    }

    fclose(logf);
    return 0;
}
