#include <stdio.h>
#include <string.h>

const int LEN = 1000000;

int main(void) {
    int K;
    int ln; // len of current word
    int sln=0; // sum of lens of words in current paragraph
    int n=0; // num of words in current paragraph
    scanf("%d\n", &K);
    char w[LEN], *pw;
    fgets(w, LEN, stdin);
    ln = strnlen(w, LEN);
    w[ln-1] = '\0'; // replace tailing \n by \0
    pw = strtok(w, " "); // the first word
    char *ppw[100000]; // buffer of pointers to words of current paragraph
    while (1) {
        if (pw != NULL)
            ln = strnlen(pw, LEN);
        if (pw == NULL || sln + ln + n > K) { // end of input line or the current word is out of paragraph - print
            if (n == 1) { // single word in paragraph
                printf("%s", ppw[0]);
                for (int i=0; i<K-sln; i++)
                    printf(" ");
                printf("\n");
            } else { // some words in paragraph
                int p = (K - sln) / (n - 1); // minimal number of spaces between words
                int q = (K - sln) % (n - 1); // number of intervals with +1 space
                for (int i=0; i<q; i++) { // words and extended spaces
                    printf("%s", ppw[i]);
                    for (int j=0; j<p+1; j++)
                        printf(" "); // p+1 spaces
                }
                for (int i=q; i<n-1; i++) { // words with minimal spaces
                    printf("%s", ppw[i]);
                    for (int j=0; j<p; j++)
                        printf(" ");  // p spaces
                }
                printf("%s\n", ppw[n-1]); // final word of the paragraph
            }
            if (pw == NULL) // end of input line
                break;
            ppw[0] = pw; // current word is the first word in next paragraph
            sln = ln;
            n = 1;
        } else { // add the word to the paragraph
            ppw[n++] = pw; // save its pointer
            sln += ln; // add its len to the sum
        }
        pw = strtok(NULL, " "); // find the next word
    }
}
