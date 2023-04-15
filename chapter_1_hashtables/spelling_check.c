#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 100000

int prefix_length(char str1[], char str2[]) {
  int index = 1;
  while (str1[index] == str2[index]) {
    index++;
  }
  return --index;
}

int suffix_legth(char str1[], char str2[], int str1_len, int len_diff) {
  int index = str1_len;
  while (str1[index] == str2[index - len_diff] && index >= len_diff) {
    index--;
  }
  return str1_len - index;
}

int main(void) {
  static char str1[SIZE + 2], str2[SIZE + 2];
  gets(&str1[1]);
  gets(&str2[1]);

  int len = strlen(&str1[1]);
  int str2_len = strlen(&str2[1]);

  int diff = len - str2_len;
  diff = diff < 0 ? -diff : diff;

  int prefix = prefix_length(str1, str2);
  int suffix = suffix_legth(str1, str2, len, diff);
  int total = (prefix + 1) - (len - suffix) + 1 + diff;
  if (total < 0) {
    total = 0;
  }

  printf("%d\n", total);
  for (int index = 0; index < total; index++) {
    printf("%d", index + len - suffix);
    if (index < total - 1) {
      printf(" ");
    } else {
      printf("\n");
    }
  }
  return 0;
}
