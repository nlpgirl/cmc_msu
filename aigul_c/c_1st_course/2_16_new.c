#include <stdio.h>

int main(void){
    unsigned int n, x, mas[3], xorall = 0, flag = 0, tc, xorvr = 0, count = 0, mask = 1;
    mas[0] = 101; mas[1] = 0; mas[2] = 0;
    scanf("%d", &n);
    unsigned int a[n];
    //считаем массив
    for(int i = 0; i < n; i++){
       scanf("%u", &x);
       a[i] = x;
    }
    //найдем хор всех элементов
    for(int i=0; i<n; i++){
        xorall ^= a[i];
    }
    //проходимся по каждому биту xorall при помощи маски
    while(flag==0){
        mask = 1;
        mask <<= count;
        tc = xorall & mask;
        xorvr = 0;
        for(int i=0; i<n; i++){ //найдем хор всех элементов, у которых на месте count находится соответствующий бит xorall
            if((a[i]&mask) == tc){
                xorvr ^= a[i];
            }
        }
        if(xorvr != xorall){
            mas[0] = xorvr;
            flag = 1;
        }

        count += 1;
    }
    for(int i=0; i<n; i++){ //обнулим в массиве найденный элемент
        if(a[i]==xorvr){
            a[i] = 0;
            break;
        }
    }
    //printf("first %u %u %u\n", mas[0], mas[1], mas[2]);
    unsigned int xorost = 0;
    xorost = xorall ^ mas[0];
    flag = 0;
    count = 0;
    //unsigned int odin = 1;
    while(flag == 0){
        mask = 1;
        mask <<= count;
        xorvr = 0;
        tc = xorost & mask;
        for(int i=0; i<n; i++){
            if((a[i]&mask)==mask){
                xorvr ^= a[i];
            }
            if(xorvr != xorost && tc == mask){
                mas[1] = xorvr;
                flag = 1;
            }
        }
        count++;
    }
    mas[2] = mas[1]^xorost;
    unsigned int max, min;
    max = min = mas[0];
    for(int i=1; i<3; i++){
        if (mas[i] > max)
            max = mas[i];
        if (mas[i] < min)
            min = mas[i];
    }
    printf("%u %u %u", min, mas[0]+mas[1]+mas[2]-min-max, max);

return 0;
}
