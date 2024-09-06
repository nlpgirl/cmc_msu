#include <stdio.h>

int main(void) {
  int n, x = 0;
  long int maxim = -10000000000000000;
  long int minim = 10000000000000000;
  scanf("%d", &n);
  int mas[n];
  for (x = 0; x < n; x ++)
  {
      scanf("%d", &mas[x]);
  }
  for (x = 0; x < n; x ++)
  {
    if (minim > mas[x])
            minim = mas[x];
    if (maxim < mas[x])
            maxim = mas[x];
  }
  printf("%ld", maxim-minim);
  return 0;
}
