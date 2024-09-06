#include <stdio.h>
#include <string.h>

void print_line(int k, char w[1000002][k + 2], int l, int r) {
    int amount_w = r - l + 1;
    int w_len = 0;
    for (int i = l; i <= r; ++i)
        w_len += strlen(w[i]);
    int amount_sp = k - w_len;
    int curr_sum = 0;
    printf("%s", w[l]);
    for (int i = 0; i < amount_w - 1; ++i) {
        int x = (amount_sp - curr_sum) / (amount_w - 1 - i);
        if ((amount_sp - curr_sum) % (amount_w - 1 - i))
            ++x;
        curr_sum += x;
        for (int j = 0; j < x; ++j)
            printf(" ");
        printf("%s", w[l + i + 1]);
    }
    if (r == l) {
        for (int i = 0; i < amount_sp; ++i)
            printf(" ");
    }
    printf("\n");
}

int main(void) {
    int k;
    scanf("%d\n", &k);
    char s[1000002];
    fgets(s, 1000002, stdin);
    if (s[strlen(s) - 1] == '\n')
        s[strlen(s) - 1] = '\0';
    int amount = 0, curr = 0;
    char w[1000002][k + 2];
    for (int i = 0; i < strlen(s); ++i) {
        if (s[i] != ' ') {
            w[amount][curr++] = s[i];
        } else {
            w[amount++][curr] = '\0';
            curr = 0;
        }
    }
    ++amount;
    int len_curr = 0, l_curr = 0, r_curr = 0;
    while (l_curr < amount) {
        len_curr += strlen(w[l_curr]);
        r_curr = l_curr;
        while (r_curr < amount - 1) {
            int t = strlen(w[r_curr + 1]);
            if (len_curr + 1 + t > k) {
                break;
            }
            len_curr += 1 + t;
            ++r_curr;
        }
        if (r_curr == amount)
            --r_curr;
        print_line(k, w, l_curr, r_curr);
        l_curr = r_curr + 1;
        len_curr = 0;
    }
    return 0;
}
