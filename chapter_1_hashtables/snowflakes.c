#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define SNOWFLAKE_SIDES 6
#define MAX_SIZE 100000
#define hashsize(n) ((unsigned long)1 << (n))
#define hashmask(n) (hashsize(n) - 1)

typedef struct snowflake_node {
  int snowflake[SNOWFLAKE_SIDES];
  struct snowflake_node *next;
} snowflake_node;

int code(int snowflake[]) {
  int sum = 0;
  for (int side = 0; side < SNOWFLAKE_SIDES; side++) {
    sum += snowflake[side];
  }
  return sum % MAX_SIZE;
}

unsigned long oaat(char *key, unsigned long len, unsigned long bits) {
  unsigned long hash = 0, i = 0;
  for (; i < len; i++) {
    hash += key[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }
  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);
  printf("Hash: %u\n", hash);
  printf("Masked hash: %u\n", hash & hashmask(bits));
  return hash & hashmask(bits);
}

int identical_left(int snow1[], int snow2[], int start) {
  int snow2_index;
  for (int offset = 0; offset < SNOWFLAKE_SIDES; offset++) {
    snow2_index =
        start - offset < 0 ? start - offset + SNOWFLAKE_SIDES : start - offset;
    if (snow1[offset] != snow2[snow2_index]) {
      return 0;
    }
  }
  return 1;
}

int identical_right(int snow1[], int snow2[], int start) {
  for (int offset = 0; offset < SNOWFLAKE_SIDES; offset++) {
    if (snow1[offset] != snow2[(start + offset) % SNOWFLAKE_SIDES]) {
      return 0;
    }
  }
  return 1;
}

int are_identical(int snow1[], int snow2[]) {
  for (int start = 0; start < SNOWFLAKE_SIDES; start++) {
    if (identical_left(snow1, snow2, start) ||
        identical_right(snow1, snow2, start)) {
      return 1;
    }
  }
  return 0;
}

int new_are_identical(int snow1[], int snow2[]) {
  for (int start = 0; start < SNOWFLAKE_SIDES; start++) {
    if (identical_left(snow1, snow2, start) ||
        identical_right(snow1, snow2, start)) {
      return 1;
    }
  }
  return 0;
}

void identify_identical(int snowflakes[][SNOWFLAKE_SIDES], int n) {
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      if (are_identical(snowflakes[i], snowflakes[j])) {
        printf("Twin integers found: %d %d\n", i, j);
      }
    }
  }
  printf("No two integers are alike\n");
}

void new_identify_identical(snowflake_node *snowflakes[]) {
  snowflake_node *node1, *node2;
  for (int i = 1; i < MAX_SIZE; i++) {
    node1 = snowflakes[i];
    while (node1 != NULL) {
      node2 = node1->next;
      while (node2 != NULL) {
        if (new_are_identical(node1->snowflake, node2->snowflake)) {
          printf("Two identical snowflakes are found\n");
          return;
        }
        node2 = node2->next;
      }
      node1 = node1->next;
    }
  }
  printf("No two snowflakes are alike\n");
}

int main(void) {
  static snowflake_node *snowflakes[MAX_SIZE] = {NULL};
  snowflake_node *snow;
  int numberOfSnowflakes, snowflake_code;
  printf("Number of snowflakes to compare: ");
  scanf("%9d", &numberOfSnowflakes);
  for (int snowflake = 0; snowflake < numberOfSnowflakes; snowflake++) {
    snow = malloc(sizeof(snowflake_node));
    if (snow == NULL) {
      fprintf(stderr, "malloc error\n");
      exit(EXIT_FAILURE);
    }
    for (int side = 0; side < SNOWFLAKE_SIDES; side++) {
      scanf("%4d", &(snow->snowflake[side]));
    }
    snowflake_code = code(snow->snowflake);
    printf("%u\n", snowflake_code);
    snow->next = snowflakes[snowflake_code];
    snowflakes[snowflake_code] = snow;
  }
  new_identify_identical(snowflakes);
  return 0;
}