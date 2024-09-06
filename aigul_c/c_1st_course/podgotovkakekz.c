#include <stdio.h>
#include <string.h>

char* fibocci(char* s){
    int fib[100], i = 2, len, curstop = 0, count=0, fibind = 0;
    len = strlen(s);
    fib[0] = 0;
    fib[1] = 1;
    fib[2] = 2;
    while(fib[i] <= strlen){
        i++;
        fib[i] = fib[i-1] + fib[i-2];
    }
    char *newstr = malloc(strlen+i+2);
    for(int i=0; i<=len; i++){
        if(i == curstop){
            newstr[count] = s[i];
            count++;
            newstr[count] = ' ';
            fibind++;
            curstop = fib[fibind];
        }
        else{
            newstr[count] = s[i];
        }
        count++;
    }
    free(s);
    return newstr;
}

int main(void){
    char *s = malloc(10);
    scanf("%s", s);
    printf(fibocci(s));
    return 0;
}
