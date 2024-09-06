#include <stdio.h>
#include <getopt.h>

int
main(int argc, char* argv[]){
    int x, pos = 0, neg = 0;
    while(scanf("%d", &x)){
        if(x > 0)
            pos += x;
        else
            neg += x;
    }
    printf("%d\n%d\n", pos, neg);
    return 0;
}
