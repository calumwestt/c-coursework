//Program to be able to traverse a binary search tree using different methods.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <limits.h>
#include <assert.h>

//Initialise node struct
struct node {
  int key;
  struct node *left;
  struct node *right;
};
typedef struct node node;

//Checks that the terminal inputs are valid
bool valid(const char length[], long num, char *ptr) {
  if (length[0] == '0') return false;
  if (num > INT_MAX | num <= 0) return false;
  if (*ptr == '\0' | *ptr == '\n') return true;
  else return false;
}

//Converts the terminal input into integers so the values can
//be added to the tree
double convert(const char length[]) {
    char *ptr;
    int num;
    num = strtol(length, &ptr, 10);
    if (valid(length, num, ptr)) return num;
    else return -1;
}

//Creates a new node for the binary search tree
node *newNode(int item) {
  node *temp =  malloc(sizeof(node));
  temp -> key = item;
  temp -> left = temp -> right = NULL;
  return temp;
}

//Traverse throught the tree to free each node
void freeTree(node *t) {
  if (t != NULL) {
    freeTree(t -> left);
    freeTree(t -> right);
    free(t);
  }
}

//Pre order traversal
void preorder(node *root) {
  if (root != NULL) {
    printf("%d, ", root -> key);
    preorder(root -> left);
    preorder(root -> right);
  }
}

//In order traversal
void inorder(node *root) {
  if (root != NULL) {
    inorder(root -> left);
    printf("%d, ", root -> key);
    inorder(root -> right);
  }
}

//Post order traversal
void postorder(node *root) {
  if (root != NULL) {
    postorder(root -> left);
    postorder(root -> right);
    printf("%d, ", root -> key);
  }
}

//Inserts a new node into the binary search tree
node *insert(node *n, int key) {
  if (n == NULL) return newNode(key);
  if (key < n -> key) n -> left = insert(n -> left, key);
  else if (key > n -> key) n -> right = insert(n -> right, key);
  return n;
}

// Gets user input for items for tree.
node *createTree() {
  node *t = NULL;
  return t;
}

//------------------------------------------------------------------------------

void test_newnode() {
  node *n = newNode(-1);
  assert(n != NULL);
  free(n);
}

void run_test() {
  test_newnode();
  printf("All tests passed.\n");
}

// Driver Program to test above functions
int main(int n, char *args[n]) {
  setbuf(stdout, NULL);
  if (n == 1) run_test();
  else {
    node *t = createTree();
    for (int i = 1; i < n; i++) {
      t = insert(t, convert(args[i]));
    }
    printf("Tree traversed via pre-order: ");
    preorder(t);
    printf("\nTree traversed via in-order: ");
    inorder(t);
    printf("\nTree traversed via post-order: ");
    postorder(t);
    printf("\n");
    freeTree(t);
  }
  return 0;
}
