#include <stdio.h>
int main(void){
    int mas[10000];
    int x, i = 0;
    scanf("%d", &x);
    while (x != 0){
       mas[i] = x;
       scanf("%d", &x);
       i++;
    }
    for (int j=0; j < i; j ++){
       if (j%2 == 0)
           printf("%d ", mas[j]);
    }
    for (int j=i-1; j>=0; j--){
       if (j%2)
           printf("%d ", mas[j]);
    }

return 0;
}
