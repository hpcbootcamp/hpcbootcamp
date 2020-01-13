#include <stdio.h>
#define MAX 500000

/* This program is from "More Programming Pearls", Jon Bentley, 1988 */

int prime(int n);

int prime(int n) {
  int i;

  for (i= 2; i < n; i++)
    if (n % i == 0)
      return 0;

  return 1;
}

int main() {
  int i;

  for (i= 2; i <= MAX; i++)
    if (prime(i))
      printf("%d is prime\n", i);

  return 0;
}
