#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int main(void){
int n, a, b;
float maxs = 0, ts;
scanf("%d", &n);
int x[n], y[n];
for(int i = 0; i < n; i++){
    scanf("%d %d", &a, &b);
    x[i] = a;
    y[i] = b;
}
for(int i = 0; i < n-2; i++){
    for(int j = i; j < n-1; j++){
        for(int k = j; k < n; k++){
            ts = 0.5 * (abs((x[j]-x[i])*(y[k]-y[i])-(x[k]-x[i])*(y[j]-y[i])));
            if (ts > maxs)
                    maxs = ts;
        }
    }
}
printf("%.6f", maxs);

return 0;
}
