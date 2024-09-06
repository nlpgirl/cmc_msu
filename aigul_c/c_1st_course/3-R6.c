#include <stdio.h>


int main(void){
    unsigned int a, min = 0xffffffff;
    scanf("%u", &a);
    while(a != 0){
        if(a < min) min = a;
        scanf("%u", &a);
    }
    printf("%u", min);

    return 0;
}
