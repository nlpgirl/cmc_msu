#include <stdio.h>
#include <stdlib.h>
#include <math.h>

unsigned f(unsigned x)
{
    unsigned ans;
    ans = x * (int)(pow(x, 0.5));
    return ans;
}

unsigned g(unsigned x)
{
    int flag = 0;
    unsigned nach, ans;
    nach = (int)(pow(x,0.666))-5;
    while(flag == 0){
        if (f(nach) == x){
            ans = nach;
            flag = 1;
        }
        nach += 1;
    }
    return ans;

}

int main(void){
unsigned x;
scanf("%u", &x);
double xn = (double)x;
printf("%u", g(xn));

return 0;
}
