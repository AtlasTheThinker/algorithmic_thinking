#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 10
#define MAX_NODES 1000
#define hashsize(n) ((unsigned long)1 << (n))
#define hashmask(n) (hashsize(n) - 1)

typedef struct node {
  char *name;
  int num_children;
  struct node **children;
  int score;
} node;

node *find_node(node *nodes[], int num_nodes, char *name);
void *malloc_safe(int size);
node *new_node(char *name);
int read_tree(node *nodes[], int num_lines);
int score_one(node *n, int d);
void score_all(node **nodes, int num_nodes, int d);
int compare(const void *v1, const void *v2);
void output_info(node *nodes[], int num_nodes);

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

node *find_node(node *nodes[], int num_nodes, char *name) {
  int node_code = oaat(name, MAX_NAME + 1, 9);
  node *target_node = nodes[node_code];
  return target_node;
}

void *malloc_safe(int size) {
  char *mem = malloc(size);
  if (mem == NULL) {
    fprintf(stderr, "malloc error\n");
    exit(EXIT_FAILURE);
  }
  return mem;
}

node *new_node(char *name) {
  node *n = malloc_safe(sizeof(node));
  n->name = name;
  n->num_children = 0;
  return n;
}

int read_tree(node *nodes[], int num_lines) {
  node *parent_node, *child_node;
  char *parent_name, *child_name;
  int num_children;
  int num_nodes = 0;
  for (int i = 0; i < num_lines; i++) {
    parent_name = malloc_safe(MAX_NAME + 1);
    scanf("%s", parent_name);
    scanf("%d", &num_children);
    parent_node = find_node(nodes, num_nodes, parent_name);
    if (parent_node == NULL) {
      printf("NOT FOUND PARENT NODE\n");
      parent_node = new_node(parent_name);
      int parent_node_code = oaat(parent_name, MAX_NAME + 1, 9);
      nodes[parent_node_code] = parent_node;
      num_nodes++;
    } else {
      printf("FOUND PARENT NODE\n");
      free(parent_name);
    }
    parent_node->children = malloc_safe(sizeof(node) * num_children);
    parent_node->num_children = num_children;
    for (int j = 0; j < num_children; j++) {
      child_name = malloc_safe(MAX_NAME + 1);
      scanf("%s", child_name);
      child_node = find_node(nodes, num_nodes, child_name);
      if (child_node == NULL) {
        printf("NOT FOUND CHILD NODE\n");
        child_node = new_node(child_name);
        int child_node_code = oaat(child_name, MAX_NAME + 1, 9);
        nodes[child_node_code] = child_node;
        num_nodes++;
      } else {
        printf("FOUND CHILD NODE\n");
        free(child_name);
      }
      parent_node->children[j] = child_node;
    }
  }
  return num_nodes;
}

int score_one(node *n, int d) {
  if (d == 1) {
    return n->num_children;
  }
  int total = 0;
  for (int i = 0; i < n->num_children; i++) {
    total += score_one(n->children[i], d - 1);
  }
  return total;
}

void score_all(node **nodes, int num_nodes, int d) {
  for (int i = 0; i < MAX_NODES; i++) {
    if (nodes[i]->name) {
      nodes[i]->score = score_one(nodes[i], d);
    }
  }
}

int compare(const void *v1, const void *v2) {
  const node *n1 = *(const node **)v1;
  const node *n2 = *(const node **)v2;
  if (n1->score < n2->score) {
    return 1;
  }
  if (n1->score > n2->score) {
    return -1;
  }
  return strcmp(n1->name, n2->name);
}

void output_info(node *nodes[], int num_nodes) {
  int i = 0;
  while (i < 3 && i < num_nodes && nodes[i]->score > 0) {
    printf("%s %d\n", nodes[i]->name, nodes[i]->score);
    i++;
    while (i < num_nodes && nodes[i]->score == nodes[i - 1]->score) {
      printf("%s %d\n", nodes[i]->name, nodes[i]->score);
      i++;
    }
  }
}

int main(void) {
  int num_cases, n, d, num_nodes;
  node **nodes = malloc_safe(sizeof(node) * MAX_NODES);
  scanf("%3d", &num_cases);
  for (int case_num = 1; case_num <= num_cases; case_num++) {
    printf("Tree %d:\n", case_num);
    scanf("%3d %3d", &n, &d);
    num_nodes = read_tree(nodes, n);
    score_all(nodes, num_nodes, d);
    printf("done scoring");
    qsort(nodes, num_nodes, sizeof(node *), compare);
    printf("done sorting");
    output_info(nodes, num_nodes);
    if (case_num < num_cases) {
      printf("\n");
    }
  }
  return 0;
}