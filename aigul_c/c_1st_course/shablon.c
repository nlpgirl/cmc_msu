#include <stdio.h>
#include <stdlib.h>

struct numbere {
    enum {
            SHORT,
            FLOAT
    } e;
    union {
        short s;
        float f;
    };
} num;


int
main(void){
    printf("%d", sizeof(num));
    return 0;
}
