#include <stdio.h>

void rec(int number){
    int x;
    scanf("%d", &x);
    if (x == 0){
        return;
    }
    if (number % 2 != 0){
        printf("%d ", x);
        rec(number+1);
    }
    else {
        rec(number+1);
        printf("%d ", x);
    }

}
int main(void){

    rec(1);

return 0;
}
