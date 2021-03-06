#include "hash.h"
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Defines hash_table struct, capacity is number of possible entries in table
//capacity gets doubled each time the limit is reached.
struct hash_table {
  node **array;
  int current_size;
  int max_size;
};
typedef struct hash_table hash_table;

//Defines node struct, next points to subsequent node, useful for searching
//table for values, key generated by hash function, stored in pair alongside
//value used to create key.
struct node {
  node *next;
  char *data;
  char *key;
};
typedef struct node node;

hash_table *create_table(int max_size) {
  hash_table *new_hash = malloc(sizeof(hash_table));
  new_hash -> max_size = max_size;
  new_hash -> current_size = 0;
  new_hash -> array = calloc(max_size, sizeof(node));
  return new_hash;
}

node *create_node(char *data, char *key) {
  node *new_node = malloc(sizeof(node));
  new_node -> next = NULL;
  new_node -> data = data;
  new_node -> key = key;
  return new_node;
}

void free_hash(hash_table *h) {
  int i;
  node *temp;
  node *current;
  for (i = 0; i < h -> max_size; i++) {
    for (current = h -> array[i]; current != NULL; current = temp) {
      temp = current -> next;
      free(current);
      current = temp;
    }
  }
  free(h -> array);
  free(h);
}

void free_node(node *n) {
  free(n);
}

int hash_function(const char *c, hash_table *h) {
  unsigned long key = 1;
  int size = strlen(c);
  for (int i = 0; i < size; i++) {
    key = key * 31 + c[i];
  }
  key = key % h -> max_size;
  return key;
}

void hash_again(hash_table *h) {
  hash_table *temp = create_table((h -> max_size) * 2);
  node *n;
  for (int i = 0; i < h -> max_size; i++) {
    for (n = h -> array[i]; n != NULL; n = n -> next) {
      insert(temp, n -> key, n -> data);
    }
  }
  h -> max_size = temp -> max_size;
  h -> current_size = temp -> current_size;
  node** temp_array = h -> array;
  h -> array = temp -> array;
  temp -> array = temp_array;
  free_hash(temp);
}

int get_index(hash_table *h, char *key, int index) {
  node *temp = h -> array[index];
  int list_index = 0;
  while (temp != NULL) {
    if(strcmp(temp -> key, key) == 0) return list_index;
    temp = temp -> next;
    list_index++;
  }
  return -1;
}

node *get_node(hash_table *h, int list_index, int index) {
  node *n = h -> array[index];
  for (int i = 0 ; i < list_index; i++) {
    n = n -> next;
  }
  return n;
}

bool is_list_empty(int i, hash_table *h) {
  if (h -> array[i] == NULL) return true;
  else return false;
}

bool is_table_empty(hash_table *h) {
  if (h -> current_size == 0) return true;
  else return false;
}

bool is_table_full (hash_table *h) {
  if (h -> current_size == h -> max_size) return true;
  else return false;
}

void insert(hash_table *h, char *key, char *data) {
  int index = hash_function(key, h);
  node *n = create_node(data, key);
  int list_index = get_index(h, key, index);
  if (is_list_empty(index, h) == true) {
    h -> array[index] = n;
    h -> current_size++;
  }
  else if (list_index != -1) {
    node *node = get_node(h, list_index, index);
    node -> data = data;
  }
  else {
    node *next_node = h -> array[index] -> next;
    h -> array[index] -> next = n;
    n -> next = next_node;
    h -> current_size++;
  }
  if (h -> current_size >= h -> max_size * 0.8) {
    hash_again(h);
  }
}

void del(hash_table *h, char *key) {
  int index = hash_function(key, h);
  node *n = h -> array[index];
  node *temp = n;
  if (n == NULL) {
    return;
  }
  while(n != NULL) {
    if (strcmp(n -> key, key) ==0 ) {
      if (n == h -> array[index]) {
        h -> array[index] = n -> next;
      }
      else {
        temp -> next = n -> next;
      }
      h -> current_size--;
      free(n);
      break;
    }
    temp = n;
    n = n->next;
  }
}

//------------------------------------------------------------------------------
//Testing and main function

void test_create_hash() {
  hash_table *h = create_table(16);
  assert(h != NULL);
  free_hash(h);
}

void test_create_node() {
  node *n = create_node("10", "hello");
  assert(n != NULL);
  free_node(n);
}

void test_hash_function() {
  hash_table *h = create_table(16);
  assert(hash_function("10", h) == 0);
  assert(hash_function("11", h) == 1);
  assert(hash_function("12", h) == 2);
  assert(hash_function("13", h) == 3);
  assert(hash_function("14", h) == 4);
  assert(hash_function("15", h) == 5);
  assert(hash_function("16", h) == 6);
  assert(hash_function("17", h) == 7);
  assert(hash_function("18", h) == 8);
  assert(hash_function("19", h) == 9);
  free_hash(h);
}

void test_hash_again() {
  hash_table *h = create_table(8);
  insert(h, "31", "Hello");
  insert(h, "32", "My");
  insert(h, "43", "Name");
  insert(h, "53", "Is");
  insert(h, "54", "John");
  insert(h, "58", "Smith");
  assert(h -> max_size == 8);
  insert(h, "42", "And");
  assert(h -> max_size == 16);
  free_hash(h);
}

void test_is_table_empty() {
  hash_table *h = create_table(16);
  assert(is_table_empty(h) == true);
  free_hash(h);
}

void test_is_table_full() {
  hash_table *h = create_table(16);
  assert(is_table_full(h) == false);
  free_hash(h);
}

void test_insert() {
  hash_table *h = create_table(16);
  insert(h, "31", "Hello");
  assert(strcmp(h -> array[hash_function("31", h)] -> data, "Hello") == 0);
  insert(h, "32", "My");
  assert(strcmp(h -> array[hash_function("32", h)] -> data, "My") == 0);
  insert(h, "43", "Name");
  assert(strcmp(h -> array[hash_function("43", h)] -> next -> data, "Name")==0);
  insert(h, "53", "Is");
  assert(strcmp(h -> array[hash_function("53", h)] -> next -> data, "Is")==0);
  insert(h, "54", "John");
  assert(strcmp(h -> array[hash_function("54", h)]->next->data, "John")== 0);
  assert(strcmp(h->array[hash_function("43", h)]->next->next->data,"Name")==0);
  insert(h, "57", "Smith");
  free_hash(h);
}

void test_del() {
  hash_table *h = create_table(16);
  insert(h, "31", "Hello");
  insert(h, "32", "My");
  insert(h, "43", "Name");
  insert(h, "53", "Is");
  insert(h, "54", "John");
  del(h, "31");
  assert(strcmp(h -> array[hash_function("31", h)] -> data, "Is") == 0);
  del(h, "32");
  assert(strcmp(h -> array[hash_function("32", h)] -> data, "John") == 0);
  del(h, "43");
  assert(strcmp(h -> array[hash_function("32", h)] -> data, "John") == 0);
  del(h, "53");
  assert(h -> array[hash_function("53", h)] == NULL);
  del(h, "54");
  assert(h -> array[hash_function("54", h)] == NULL);
  free_hash(h);
}

void run_test() {
  test_create_hash();
  test_create_node();
  test_hash_function();
  test_hash_again();
  test_is_table_full();
  test_is_table_empty();
  test_insert();
  test_del();
  printf("All modules okay.\n");
}

int main() {
  run_test();
  return 0;
}
