#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_BITS 17
#define WORD_LENGTH 16
#define hashsize(n) ((unsigned long)1 << (n))
#define hashmask(n) (hashsize(n) - 1)

typedef struct word_node {
  char **word;
  struct word_node *next;
} word_node;

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
  return hash & hashmask(bits);
}

int in_hash_table(word_node *hash_table[], char *find, unsigned find_len) {
  unsigned word_code;
  word_node *wordptr;
  word_code = oaat(find, find_len, NUM_BITS);
  wordptr = hash_table[word_code];
  while (wordptr) {
    if ((strlen(*(wordptr->word)) == find_len) &&
        (strncmp(*(wordptr->word), find, find_len) == 0)) {
      return 1;
    }
    wordptr = wordptr->next;
  }
  return 0;
}

void identify_compound_words(char *words[], word_node *hash_table[],
                             int total_words) {
  unsigned len;
  for (int word_index = 0; word_index < total_words; word_index++) {
    len = strlen(words[word_index]);
    for (int word_length = 1; word_length < len; word_length++) {
      if (in_hash_table(hash_table, words[word_index], word_length) &&
          in_hash_table(hash_table, &words[word_index][word_length],
                        len - word_length)) {
        printf("%s\n", words[word_index]);
        break;
      }
    }
  }
}

char *read_line(int size) {
  char *str;
  int ch;
  int len = 0;
  str = malloc(size);
  if (str == NULL) {
    fprintf(stderr, "mallor error\n");
    exit(1);
  }
  while ((ch = getchar()) != EOF && (ch != '\n')) {
    str[len++] = ch;
    if (len == size) {
      size *= 2;
      str = realloc(str, size);
      if (str == NULL) {
        fprintf(stderr, "mallor error\n");
        exit(1);
      }
    }
  }
  str[len] = '\0';
  return str;
}

int main(void) {
  static char *words[1 << NUM_BITS] = {NULL};
  static word_node *hash_table[1 << NUM_BITS] = {NULL};
  int total = 0;
  char *word;
  word_node *wordptr;
  unsigned length, word_code;
  word = read_line(WORD_LENGTH);
  while (*word) {
    words[total] = word;
    wordptr = malloc(sizeof(word_node));
    if (wordptr == NULL) {
      fprintf(stderr, "malloc error\n");
      exit(EXIT_FAILURE);
    }
    length = strlen(word);
    word_code = oaat(word, length, NUM_BITS);
    wordptr->word = &words[total];
    wordptr->next = hash_table[word_code];
    hash_table[word_code] = wordptr;
    word = read_line(WORD_LENGTH);
    total++;
  }
  identify_compound_words(words, hash_table, total);
  return 0;
}