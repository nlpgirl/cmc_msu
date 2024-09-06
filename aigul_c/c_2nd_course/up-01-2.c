#include <stdio.h>
#include <ctype.h>

enum sixbit_codes
{
    code_of_start = 0,
    code_of_0 = 1,
    code_of_a = code_of_0 + '9' - '0' + 1,
    code_of_A = code_of_a + 'z' - 'a' + 1,
    code_of_end = code_of_A + 'Z' - 'A' + 1
};

int
char_to_sixbit(char c)
{
    if ('0' <= c && c <= '9') {
        return c - '0' + code_of_0;
    } else if ('a' <= c && c <= 'z') {
        return c - 'a' + code_of_a;
    } else if ('A' <= c && c <= 'Z') {
        return c - 'A' + code_of_A;
    } else {
        return -1;
    }
}

char
sixbit_to_char(int code)
{
    if (code >= code_of_0 && code < code_of_a) {
        return '0' + code - code_of_0;
    } else if (code >= code_of_a && code < code_of_A) {
        return 'a' + code - code_of_a;
    } else if (code >= code_of_A && code < code_of_end) {
        return 'A' + code - code_of_A;
    } else if (code == code_of_start) {
        return '@';
    } else if (code == code_of_end) {
        return '#';
    } else {
        return '?';
    }
}

int
main(void)
{
    int int_c;
    char c;
    while ((int_c = getchar()) != EOF) {
        c = int_c;
        if (!isalnum(c)) {
            continue;
        }
        int code = char_to_sixbit(c);
        code = code ^ (1 << 3);
        code = code & (~(1 << 2));

        putchar(sixbit_to_char(code));
    }
    return 0;
}
