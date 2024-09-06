#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int hod(char *st, char *now, int count){ //функция проверяет правильность хода
    if((st[0] == now[0]) && (st[1] == now[1])) //крайний случай, когда одинаковые предыдущий и новый ходы
        return 0;
    if (count % 2 == 0) { //Garik
        if((st[0] == now[0]) || (st[1] == now[1]) || (abs(now[0]-st[0]) == abs(now[1]-st[1])))
            return 1;
        else return 0;
    }
    else //Tolik
        if(((abs(now[0]-st[0]) == 1) && (abs(now[1]-st[1]) == 2)) || ((abs(now[0]-st[0]) == 2) && (abs(now[1]-st[1]) == 1)))
            return 1;
        else return 0;
}

int main(void){
    char st[3], now[3]; //массив старого хода, нового хода
    st[0] = 'A';
    st[1] = '1';
    scanf("%s", now);
    if(now[0] == 'X'){ //крайний случай, когда сразу ввели Х
        printf("Cheaters");
        return 0;
    }
    int count = 1;
    int flag = 0;
    while (now[0] != 'X'){
        if (hod(st, now, count) == 0){
            flag = 1;
        }
        st[0] = now[0]; //меняю элементы массива st
        st[1] = now[1];
        scanf("%s", now);
        count += 1;

        if((st[0] == 'H') && (st[1] == '8')){
            if (now[0] == 'X'){
                if (flag == 0){
                    if (count%2 == 0) printf("Garik");
                    else printf("Tolik");
                }
            }
            else flag = 1; //флаг поднимаем для случая, когда после Н8 ввели еще ходы
        }
        else if (now[0] == 'X'){
            printf("Cheaters");
        }
    }
    if (flag == 1){
        printf("Cheaters");
    }
    return 0;
}
