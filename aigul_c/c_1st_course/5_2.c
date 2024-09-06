#include <stdio.h>
#include <stdlib.h>

int main(void){
    int n, ans = 0, size, maxsize = 0, element, flag = 1, flag1 = 1, therewasamax = 0;
    long long int trace = 0, maxtrace = 0, previousmaxsize = 0;
    scanf("%d", &n);
    int **a = NULL;
    int **amax = NULL;
    for (int i=0; i<n; i++){
        flag1 = 1;
        scanf("%d", &size);
        trace = 0;
        if(size == 1){
            scanf("%lld", &trace);
            ans = trace;
            flag1 = 0;
        }
        if(flag1){
            a = (int**) malloc(sizeof(int*)*size);
            for(int j=0; j<size; j++){
                a[j] = (int*) malloc(sizeof(int)*size);
            }
            for(int j=0; j<size; j++){
                for(int z=0; z<size; z++){
                    scanf("%d", &element);
                    a[j][z] = element;
                }
            }
            for(int j=0; j<size; j++){
                trace += a[j][j];
            }
            flag1 = 1;
        }

        if(flag || (trace > maxtrace)){
            maxtrace = trace;
            if (flag) flag = 0;
            previousmaxsize = maxsize;
            maxsize = size;
            if(flag1){
                if(therewasamax){
                    for(int j=0; j<previousmaxsize; j++){
                        free(amax[j]);
                    }
                    free(amax);
                }
                therewasamax = 1;
                amax = (int**) malloc(sizeof(int*)*size);
                for(int j=0; j<size; j++){
                    amax[j] = (int*) malloc(sizeof(int)*size);
                }
                for(int j=0; j<size; j++){
                    for(int z=0; z<size; z++){
                        amax[j][z] = a[j][z];
                    }
                }
            }
        }
        if(flag1){
            for(int j=0; j<size; j++){
                free(a[j]);
            }
            free(a);
        }


    }
    if(maxsize == 1){
        printf("%d", ans);
        return 0;
    }
    for(int i=0; i<maxsize; i++){
        for(int j=0; j<maxsize; j++){
            printf("%d ", amax[i][j]);
        }
        printf("\n");
    }
    return 0;
}
