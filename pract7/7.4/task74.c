#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    FILE *f;
    char buf[BUFSIZ];
    int line = 0, i, c;
    if(argc < 2) return 1;
    for(i = 1; i < argc; i++){
        f = fopen(argv[i], "r");
        if(!f) continue;
        while(fgets(buf, sizeof(buf), f)){
            fputs(buf, stdout);
            if(++line == 20){
                printf("--More--");
                getchar();
                line = 0;
            }
        }
        fclose(f);
    }
    return 0;
}
