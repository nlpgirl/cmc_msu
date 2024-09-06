#include <stdio.h>

// Рекурсивная функция для вычисления количества единиц в троичной записи числа
int F(unsigned int x) {
    if (x == 0) {
        return 0;
    }
    int quotient = x / 3; // частное от деления на 3
    int remainder = x % 3; // остаток от деления на 3
    if (remainder == 1) {
        return F(quotient) + 1;
    } else if (remainder == 2) {
        return F(quotient + 1) + 1;
    } else { // remainder == 0
        return F(quotient);
    }
}

int main() {
    unsigned int x;
    scanf("%u", &x); // ввод числа x
    int result = F(x); // вызов рекурсивной функции
    printf("%d\n", result); // вывод результата
    return 0;
}
