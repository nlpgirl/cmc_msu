#include <stdio.h>
#include <math.h>
int main(void){
    int a[8], b[8], x, ans = 0, maxa, maxb;
    for(int i=0; i < 8; i++)
    {
      scanf("%d", &x);
      a[i] = x;
    }
    for(int i=0; i < 8; i++)
    {
      scanf("%d", &x);
      b[i] = x;
    }
    maxa = a[0];
    maxb = b[0];
    for(int i=1; i < 8; i++){
        if(a[i] > maxa)
            maxa = a[i];
    }
    for(int i=1; i < 8; i++){
        if(b[i] > maxb)
            maxb = b[i];
    }
    if(maxa == maxb){
    for(int i=0; i < 8; i++)
    {
       for(int j=0; j < 8; j++)
        {
        ans += fmin(a[i], b[j]);
        }
    }
    printf("%d", ans);
    }
    else {printf("%d", -1);}
    //printf("maxa=%d maxb=%d", maxa, maxb);
return 0;
}
