#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Структура для зберігання сигнатур файлів
typedef struct {
    const char *ext;          // Розширення файлу
    uint8_t start[2];         // Початкова сигнатура (2 байти)
    uint8_t end[2];           // Кінцева сигнатура (2 байти)
} FileSignature;

FileSignature signatures[] = {
    {".jpg", {0xFF, 0xD8}, {0xFF, 0xD9}},  // JPEG
    {".png", {0x89, 0x50}, {0xAE, 0x42}},  // PNG
    {".txt", {0x00, 0x00}, {0x00, 0x00}}   // Текст
};
const int sig_count = sizeof(signatures) / sizeof(FileSignature);

int main(int argc, char *argv[]) {
    if (argc != 3) return 1;
    FILE *in = fopen(argv[1], "rb");
    FILE *out = NULL;
    if (!in) return 1;

    uint8_t prev = 0, cur;
    int found = 0;
    char out_ext[5] = ".bin"; // Тип файлу за замовчуванням
    int sig_idx = -1;

    while (fread(&cur, 1, 1, in)) {
        if (!found) {
            for (int i = 0; i < sig_count; i++) {
                if (prev == signatures[i].start[0] && cur == signatures[i].start[1]) {
                    found = 1;
                    sig_idx = i;
                    snprintf(out_ext, 5, "%s", signatures[sig_idx].ext);
                    out = fopen(argv[2], "wb");
                    if (!out) {
                        fclose(in);
                        return 1;
                    }
                    fputc(prev, out);
                    fputc(cur, out);
                    break;
                }
            }
        } else if (found && sig_idx >= 0) {
            fputc(cur, out);
            if (prev == signatures[sig_idx].end[0] && cur == signatures[sig_idx].end[1]) {
                found = 0;
                fclose(out);
                printf("Recovered to %s%s\n", argv[2], out_ext);
                break;
            }
        }
        prev = cur;
    }

    fclose(in);
    if (out) fclose(out);
    if (!found) {
        if (out) {
            fclose(out);
            remove(argv[2]);
        }
        printf("No recognizable file found\n");
        return 1;
    }
    return 0;
}
