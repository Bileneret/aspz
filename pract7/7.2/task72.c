#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <unistd.h>

void print_mode(mode_t m, char *s){
    s[0] = S_ISDIR(m)? 'd' :
           S_ISLNK(m)? 'l' :
           '-';
    s[1] = (m & S_IRUSR)? 'r':'-';
    s[2] = (m & S_IWUSR)? 'w':'-';
    s[3] = (m & S_IXUSR)? 'x':'-';
    s[4] = (m & S_IRGRP)? 'r':'-';
    s[5] = (m & S_IWGRP)? 'w':'-';
    s[6] = (m & S_IXGRP)? 'x':'-';
    s[7] = (m & S_IROTH)? 'r':'-';
    s[8] = (m & S_IWOTH)? 'w':'-';
    s[9] = (m & S_IXOTH)? 'x':'-';
    s[10]= '\0';
}

int main(void){
    DIR *d = opendir(".");
    struct dirent *e;
    struct stat st;
    char mode[11], timebuf[20];
    struct passwd *pw;
    struct group *gr;
    struct tm *tm;

    if(!d) return 1;
    while((e = readdir(d))){
        if(e->d_name[0]=='.') continue;
        if(stat(e->d_name, &st)) continue;
        print_mode(st.st_mode, mode);
        pw = getpwuid(st.st_uid);
        gr = getgrgid(st.st_gid);
        tm = localtime(&st.st_mtime);
        strftime(timebuf, sizeof(timebuf), "%b %e %H:%M", tm);
        printf("%s %2lu %-8s %-8s %6lld %s %s\n",
            mode,
            (unsigned long)st.st_nlink,
            pw ? pw->pw_name : "?",
            gr ? gr->gr_name : "?",
            (long long)st.st_size,
            timebuf,
            e->d_name);
    }
    closedir(d);
    return 0;
}
