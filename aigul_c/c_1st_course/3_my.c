#include <stdio.h>
int main(void){
char a[4];
a = "abc";
for(int i = 0; i < 3; i++){
    printf("%c", a[i]);
}
//printf(a);
return 0;
}
