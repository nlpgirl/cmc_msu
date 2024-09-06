#include <stdio.h>
int main(void){
    int n, x, mas[3], xorall = 0, flag = 0, tc, xorvr = 0, xorallconst = 0;
    mas[0] = 0; mas[1] = 0; mas[2] = 0;
    scanf("%d", &n);
    int aconst[n];
    //считаем массив
    for(int i = 0; i < n; i++){
       scanf("%d", &x);
       aconst[i] = x;
    }
    //найдем хор всех элементов
    for(int i=0; i<n; i++){
        xorallconst ^= aconst[i];
    }
    //создадим копию массива aconst
    int a[n];
    for(int i = 0; i < n; i++){
       a[i] = aconst[i];
    }
    xorall = xorallconst;

    while(flag==0){
        tc = xorall % 2;
        int xorvr = 0;
        for(int i=0; i<n; i++){
            if(a[i]%2 == tc)
                xorvr ^= a[i];
            a[i] /= 2;
            }
        if (xorall != xorvr){
            mas[0] = xorvr;
            flag = 1;
        }
       xorall /= 2;
    }
    printf("mas nach %d %d %d\n", mas[0], mas[1], mas[2]);
    //создадим копию массива aconst
    int b[n];
    for(int i = 0; i < n; i++){
       b[i] = aconst[i];
    }
    int xorost;
    xorost = xorallconst^mas[0];
    flag = 0;

    while(flag == 0){
        tc = xorost % 2;
        xorvr = 0;
        for(int i = 0; i<n; i++){
            if((tc == 1) && (b[i] % 2 == 1)){
                xorvr ^= b[i];
            }
            b[i] /= 2;
        }
        if((tc == 1) &&(xorost != xorvr)){
            mas[1] = xorvr;
            flag = 1;
        }
        xorost /= 2;
    }

    mas[2] = xorvr^xorost;
    //отсортируем элементы массива
    int max, min;
    max = min = mas[0];
    for(int i=1; i<3; i++){
        if (mas[i] > max)
            max = mas[i];
        if (mas[i] < min)
            min = mas[i];
    }
    printf("unsorted mas %d %d %d\n", mas[0], mas[1], mas[2]);
    printf("%d %d %d", min, mas[0]+mas[1]+mas[2]-min-max, max);
return 0;
}


/* while(flag==0){
        tc = xorall % 2;
        if(tc == 1){
            int xor1 = 0;
            for(int i=0; i<n; i++){
                if(a[i]%2 == 1)
                    xor1 ^= a[i];
            }
            if (xorall != xor1){
                mas[count] = xor1;
                count += 1;
                flag = 1;
            }
        }
        else{
            int xor0 = 0;
            for(int i=0; i<n; i++){
                if(a[i]%2 == 0)
                    xor0 ^= a[i];
            }
            if (xorall != xor0){
                mas[count] = xor0;
                count += 1;
                flag = 1;
            }
        }
        xorall /= 2;
    }
    */
