

#include <stdio.h>
#include <stdlib.h>

#define SIZE 255
#define TEST_CASES 5

typedef struct node {
  int candy;
  struct node *left, *right;
} node;

typedef struct stack {
  node *values[SIZE];
  int highest_use;
} stack;

int max(int v1, int v2) { return v1 < v2 ? v2 : v1; }

node *new_house(int candy) {
  node *house = malloc(sizeof(node));
  if (house == NULL) {
    fprintf(stderr, "malloc error\n");
    exit(EXIT_FAILURE);
  }
  house->candy = candy;
  house->left = NULL;
  house->right = NULL;
  return house;
}

node *new_nonhouse(node *left, node *right) {
  node *nonhouse = malloc(sizeof(node));
  if (nonhouse == NULL) {
    fprintf(stderr, "malloc error\n");
    exit(EXIT_FAILURE);
  }
  nonhouse->left = left;
  nonhouse->right = right;
  return nonhouse;
}

stack *new_stack(void) {
  stack *s = malloc(sizeof(stack));
  if (s == NULL) {
    fprintf(stderr, "malloc error\n");
    exit(EXIT_FAILURE);
  }
  s->highest_use = -1;
  return s;
}

void stack_push(stack *s, node *value) {
  s->highest_use++;
  s->values[s->highest_use] = value;
}

node *stack_pop(stack *s) {
  node *remove = s->values[s->highest_use];
  s->highest_use--;
  return remove;
}

int is_stack_empty(stack *s) { return s->highest_use == 0; }

int tree_candy(node *tree) {
  int total = 0;
  stack *s = new_stack();
  while (tree) {
    if (tree->left && tree->right) {
      stack_push(s, tree->left);
      tree = tree->right;
    } else {
      total += tree->candy;
    }
    if (is_stack_empty(s)) {
      tree = NULL;
    } else {
      tree = stack_pop(s);
    }
  }
  return total;
}

int tree_streets(node *tree) {
  int sum = 4;
  if (!tree->left && !tree->right) {
    return 0;
  }
  sum += tree_streets(tree->left);
  sum += tree_streets(tree->right);
  return sum;
}

int tree_height(node *tree) {
  int sum = 1;
  if (!tree->left && !tree->right) {
    return 0;
  }
  return sum + max(tree_height(tree->left), tree_height(tree->right));
}

int recursive_tree_candy(node *tree) {
  int sum = 0;
  if (!tree->left && !tree->right) {
    return tree->candy;
  }
  sum += recursive_tree_candy(tree->left);
  sum += recursive_tree_candy(tree->right);
  return sum;
}

void tree_solve(node *tree) {
  int candy = recursive_tree_candy(tree);
  int height = tree_height(tree);
  int streets = tree_streets(tree) - height;
  printf("%d %d\n", streets, candy);
}

node *read_tree_helper(char *line, int *pos) {
  node *tree;
  tree = malloc(sizeof(node));
  if (tree == NULL) {
    fprintf(stderr, "malloc error\n");
    exit(EXIT_FAILURE);
  }
  if (line[*pos] == '(') {
    (*pos)++;
    tree->left = read_tree_helper(line, pos);
    (*pos)++;
    tree->right = read_tree_helper(line, pos);
    (*pos)++;
    return tree;
  } else {
    tree->left = NULL;
    tree->right = NULL;
    tree->candy = line[*pos] - '0';
    (*pos)++;
    if (line[*pos] != ')' && line[*pos] != ' ' && line[*pos] != '\0') {
      tree->candy = tree->candy * 10 + line[*pos] - '0';
      (*pos)++;
    }
  }
  return tree;
}

node *read_tree(char *line) {
  int pos = 0;
  return read_tree_helper(line, &pos);
}

int main(void) {
  char line[SIZE + 1];
  node *tree;
  for (int i = 0; i < TEST_CASES; i++) {
    gets(line);
    tree = read_tree(line);
    tree_solve(tree);
  }
  return 0;
}