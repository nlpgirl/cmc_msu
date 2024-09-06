#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
int isprime(int n){
    int d = 2;
    while (d*d <= n){
        if(n % d == 0){
            return 0;
        }
        d++;
    }
    return 1;
}

int main(void){
    int n;
    scanf("%d", &n);
    int *previous = NULL;
    previous = (int*)malloc(4*sizeof(int));
    previous[0] = 2; previous[1] = 3; previous[2] = 5; previous[3] = 7;
    int add[4] = {1, 3, 7, 9};
    int previousamount = 4, newamount = 0, candidate;
    int *now = NULL;

    if(n == 1){
        for(int i=0; i<previousamount; i++){
            printf("%d ", previous[i]);
        }
        return 0;
    }
    for(int numberofdigits=2; numberofdigits<=n; numberofdigits++){
        now = (int*)malloc(4*previousamount*sizeof(int));
        for(int i=0; i<previousamount; i++){
            for(int j=0; j<4; j++){
                candidate = 10*previous[i] + add[j];
                if(isprime(candidate)){
                    now[newamount] = candidate;
                    newamount++;
                }
            }
        }
        previousamount = newamount;
        newamount = 0;
        free(previous);
        previous = (int*)malloc(previousamount*sizeof(int));
        //memmove(now, previous, previousamount*sizeof(int));
        for(int i=0; i<previousamount; i++){
            previous[i] = now[i];
        }
        free(now);
        if(numberofdigits == n){
            for(int i=0; i<previousamount; i++){
                printf("%d ", previous[i]);
            }
        }
    }
    free(previous);
    return 0;
}
