#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#ifdef USE_JEMALLOC
void *je_malloc(size_t n){return malloc(n);}
void  je_free(void *p){free(p);}
#define xmalloc je_malloc
#define xfree   je_free
#else
#define xmalloc malloc
#define xfree   free
#endif

int main(void){
    char *buf=(char*)xmalloc(64);
    if(!buf) return 1;
#ifdef USE_JEMALLOC
    const char *tag="jemalloc";
#else
    const char *tag=" jemalloc analog";
#endif
    snprintf(buf,64,"It`s %s!!!!\n",tag);
    fputs(buf,stdout);
    xfree(buf);
    return 0;
}
