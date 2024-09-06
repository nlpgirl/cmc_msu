#include <stdio.h>
#include <stdlib.h>
int nod(int a, int b){
int minim;
if (a<b) minim = a;
else minim = b;
for(int i = minim; i > 0; i--){
    if ((a % i == 0) && (b % i == 0))
        return i;
}
return 1;
}
int main(void){
int n, a, b, ch, zn, x, y, z, nod1;
scanf("%d", &n);
scanf("%d %d", &a, &b);
ch = a;
zn = b;
for (int i = 0; i < n-1; i++){
    scanf("%d %d", &a, &b);
    ch = ch*b + zn*a;
    zn *= b;
}
//printf("ch is %d zn is %d\n", ch, zn);
x = ch / zn;
ch -= zn*x;
//printf("ch is %d zn is %d\n", ch, zn);
if (ch == 0){
    y = ch;
    z = 1;
}
else{
    nod1 = nod(ch, zn);
    y = ch / nod1;
    z = zn / nod1;
}
printf("%d %d %d", x, y, z);
return 0;
}
