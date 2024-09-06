#include <stdio.h>

int f(unsigned int x){
    int count = 0;
    while(x != 0){
        count += x % 2;
        x /= 2;
    }
    count = 32 - count;
    return count;
}

int main(void){
    unsigned int x;
    scanf("%u", &x);
    printf("%d", f(x));
    return 0;
}
