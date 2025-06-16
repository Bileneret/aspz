#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#define MAX_LINE_LENGTH 256

// ANSI-коди для кольорів
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_RESET "\x1b[0m"

// Виведення довідки
void print_help() {
    printf("=== Довідка по програмі maps_analyzer ===\n");
    printf("Програма аналізує вміст /proc/self/maps, виводячи інформацію про регіони пам’яті.\n");
    printf("\nВикористання:\n");
    printf("  ./maps_analyzer [-v] [-h]\n");
    printf("\nАргументи:\n");
    printf("  -v  Показати детальну інформацію (зсув, пристрій, inode).\n");
    printf("  -h  Вивести цю довідку.\n");
    printf("\nПриклад:\n");
    printf("  ./maps_analyzer -v\n");
}

void analyze_proc_maps(int verbose) {
    FILE *maps = fopen("/proc/self/maps", "r");
    if (!maps) {
        perror("Помилка відкриття /proc/self/maps");
        return;
    }

    printf("\n=== Аналіз карти пам’яті процесу ===\n");

    // Групи регіонів
    printf(ANSI_COLOR_GREEN "\nСегменти коду (виконуваний код):\n" ANSI_COLOR_RESET);
    printf("----------------------------------------\n");
    if (verbose) {
        printf("%-18s %-8s %-8s %-10s %-8s %-8s %s\n", 
               "Адреси", "Дозволи", "Розмір,KB", "Зсув", "Пристрій", "Inode", "Файл");
    } else {
        printf("%-18s %-8s %-8s %s\n", 
               "Адреси", "Дозволи", "Розмір,KB", "Файл");
    }

    char line[MAX_LINE_LENGTH];
    int code_regions = 0, heap_regions = 0, stack_regions = 0, lib_regions = 0, anon_regions = 0;

    // Перший прохід: сегменти коду
    rewind(maps);
    while (fgets(line, MAX_LINE_LENGTH, maps)) {
        unsigned long start_addr, end_addr;
        char permissions[5];
        unsigned long offset;
        char device[6];
        unsigned long inode;
        char pathname[128] = "";

        sscanf(line, "%lx-%lx %s %lx %s %lu %[^\n]", 
               &start_addr, &end_addr, permissions, &offset, device, &inode, pathname);

        if (strstr(permissions, "x") && pathname[0] != '\0') {
            long size_kb = (end_addr - start_addr) / 1024;
            if (verbose) {
                printf("%08lx-%08lx %-8s %-8ld %08lx %-10s %-8lu %s\n", 
                       start_addr, end_addr, permissions, size_kb, offset, device, inode, pathname);
            } else {
                printf("%08lx-%08lx %-8s %-8ld %s\n", 
                       start_addr, end_addr, permissions, size_kb, pathname);
            }
            code_regions++;
        }
    }

    // Другий прохід: куча
    printf(ANSI_COLOR_YELLOW "\nКуча (heap):\n" ANSI_COLOR_RESET);
    printf("----------------------------------------\n");
    if (verbose) {
        printf("%-18s %-8s %-8s %-10s %-8s %-8s %s\n", 
               "Адреси", "Дозволи", "Розмір,KB", "Зсув", "Пристрій", "Inode", "Файл");
    } else {
        printf("%-18s %-8s %-8s %s\n", 
               "Адреси", "Дозволи", "Розмір,KB", "Файл");
    }

    rewind(maps);
    while (fgets(line, MAX_LINE_LENGTH, maps)) {
        unsigned long start_addr, end_addr;
        char permissions[5];
        unsigned long offset;
        char device[6];
        unsigned long inode;
        char pathname[128] = "";

        sscanf(line, "%lx-%lx %s %lx %s %lu %[^\n]", 
               &start_addr, &end_addr, permissions, &offset, device, &inode, pathname);

        if (strstr(line, "[heap]")) {
            long size_kb = (end_addr - start_addr) / 1024;
            if (verbose) {
                printf("%08lx-%08lx %-8s %-8ld %08lx %-10s %-8lu [heap]\n", 
                       start_addr, end_addr, permissions, size_kb, offset, device, inode);
            } else {
                printf("%08lx-%08lx %-8s %-8ld [heap]\n", 
                       start_addr, end_addr, permissions, size_kb);
            }
            heap_regions++;
        }
    }

    // Третій прохід: стек
    printf(ANSI_COLOR_BLUE "\nСтек (stack):\n" ANSI_COLOR_RESET);
    printf("----------------------------------------\n");
    if (verbose) {
        printf("%-18s %-8s %-8s %-10s %-8s %-8s %s\n", 
               "Адреси", "Дозволи", "Розмір,KB", "Зсув", "Пристрій", "Inode", "Файл");
    } else {
        printf("%-18s %-8s %-8s %s\n", 
               "Адреси", "Дозволи", "Розмір,KB", "Файл");
    }

    rewind(maps);
    while (fgets(line, MAX_LINE_LENGTH, maps)) {
        unsigned long start_addr, end_addr;
        char permissions[5];
        unsigned long offset;
        char device[6];
        unsigned long inode;
        char pathname[128] = "";

        sscanf(line, "%lx-%lx %s %lx %s %lu %[^\n]", 
               &start_addr, &end_addr, permissions, &offset, device, &inode, pathname);

        if (strstr(line, "[stack]")) {
            long size_kb = (end_addr - start_addr) / 1024;
            if (verbose) {
                printf("%08lx-%08lx %-8s %-8ld %08lx %-10s %-8lu [stack]\n", 
                       start_addr, end_addr, permissions, size_kb, offset, device, inode);
            } else {
                printf("%08lx-%08lx %-8s %-8ld [stack]\n", 
                       start_addr, end_addr, permissions, size_kb);
            }
            stack_regions++;
        }
    }

    // Четвертий прохід: бібліотеки
    printf("\nБібліотеки:\n");
    printf("----------------------------------------\n");
    if (verbose) {
        printf("%-18s %-8s %-8s %-10s %-8s %-8s %s\n", 
               "Адреси", "Дозволи", "Розмір,KB", "Зсув", "Пристрій", "Inode", "Файл");
    } else {
        printf("%-18s %-8s %-8s %s\n", 
               "Адреси", "Дозволи", "Розмір,KB", "Файл");
    }

    rewind(maps);
    while (fgets(line, MAX_LINE_LENGTH, maps)) {
        unsigned long start_addr, end_addr;
        char permissions[5];
        unsigned long offset;
        char device[6];
        unsigned long inode;
        char pathname[128] = "";

        sscanf(line, "%lx-%lx %s %lx %s %lu %[^\n]", 
               &start_addr, &end_addr, permissions, &offset, device, &inode, pathname);

        if (pathname[0] != '\0' && strstr(pathname, ".so")) {
            long size_kb = (end_addr - start_addr) / 1024;
            if (verbose) {
                printf("%08lx-%08lx %-8s %-8ld %08lx %-10s %-8lu %s\n", 
                       start_addr, end_addr, permissions, size_kb, offset, device, inode, pathname);
            } else {
                printf("%08lx-%08lx %-8s %-8ld %s\n", 
                       start_addr, end_addr, permissions, size_kb, pathname);
            }
            lib_regions++;
        }
    }

    // П’ятий прохід: анонімні та інші регіони
    printf("\nІнші регіони (анонімні, vdso, vsyscall тощо):\n");
    printf("----------------------------------------\n");
    if (verbose) {
        printf("%-18s %-8s %-8s %-10s %-8s %-8s %s\n", 
               "Адреси", "Дозволи", "Розмір,KB", "Зсув", "Пристрій", "Inode", "Файл");
    } else {
        printf("%-18s %-8s %-8s %s\n", 
               "Адреси", "Дозволи", "Розмір,KB", "Файл");
    }

    rewind(maps);
    while (fgets(line, MAX_LINE_LENGTH, maps)) {
        unsigned long start_addr, end_addr;
        char permissions[5];
        unsigned long offset;
        char device[6];
        unsigned long inode;
        char pathname[128] = "";

        sscanf(line, "%lx-%lx %s %lx %s %lu %[^\n]", 
               &start_addr, &end_addr, permissions, &offset, device, &inode, pathname);

        if (!(strstr(permissions, "x") && pathname[0] != '\0') && 
            !strstr(line, "[heap]") && !strstr(line, "[stack]") && 
            !(pathname[0] != '\0' && strstr(pathname, ".so"))) {
            long size_kb = (end_addr - start_addr) / 1024;
            const char *region_type = pathname[0] != '\0' ? pathname :
                                      strstr(line, "[vdso]") ? "[vdso]" :
                                      strstr(line, "[vsyscall]") ? "[vsyscall]" :
                                      "[анонімний]";
            if (verbose) {
                printf("%08lx-%08lx %-8s %-8ld %08lx %-10s %-8lu %s\n", 
                       start_addr, end_addr, permissions, size_kb, offset, device, inode, region_type);
            } else {
                printf("%08lx-%08lx %-8s %-8ld %s\n", 
                       start_addr, end_addr, permissions, size_kb, region_type);
            }
            anon_regions++;
        }
    }

    // Підсумок
    printf("\n=== Підсумок ===\n");
    printf("Сегментів коду:    %d\n", code_regions);
    printf("Регіонів кучи:     %d\n", heap_regions);
    printf("Регіонів стеку:    %d\n", stack_regions);
    printf("Бібліотек:         %d\n", lib_regions);
    printf("Інших регіонів:    %d\n", anon_regions);
    printf("Всього регіонів:   %d\n", code_regions + heap_regions + stack_regions + lib_regions + anon_regions);

    fclose(maps);
}

int main(int argc, char *argv[]) {
    int verbose = 0;
    int opt;

    // Обробка аргументів командного рядка
    while ((opt = getopt(argc, argv, "vh")) != -1) {
        switch (opt) {
            case 'v':
                verbose = 1;
                break;
            case 'h':
                print_help();
                return 0;
            default:
                fprintf(stderr, "Використання: %s [-v] [-h]\n", argv[0]);
                return 1;
        }
    }

    analyze_proc_maps(verbose);
    return 0;
}
