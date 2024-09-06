#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

int
main(int argc, char *argv[]){
        int sum = 0;
        struct stat *buf = NULL;
        for (int i = 1; i < argc; ++i){
                if(stat(argv[i], buf) == 0){
                        if (buf->st_size % 1024 == 0 && S_ISREG(buf->st_mode) && !S_ISLNK(buf->st_mode) && (buf->st_nlink == 1)){
                                sum += buf->st_size;
                        }
                }
        }
        printf("%d\n", sum);
        return 0;
}