#include <stdio.h>
#include <stdlib.h>

int main(void){
    FILE *out = popen("more", "w");
    
    for(int i = 1; i <= 100; i++)
        fprintf(out, "Line %3d: Hello world!\n", i);
    pclose(out);
    
    return 0;
}
