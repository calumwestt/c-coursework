/*Header file for hash table module*/
#include <stdbool.h>

//Initialising structs, declared here, defined in 'hash.c'
//Hash table struct, stores lists at each entry
struct hash_table;
typedef struct hash_table hash_table;

//Node struct, stores values in list at each entry in hash table
struct node;
typedef struct node node;

//Creates a new hash table structure, sets max_size to be 16 and current_size to
//be 0, each index of the array is iterated through and is set to be NULL.
hash_table *create_table(int max_size);

//Creates a new node structure, sets next to NULL and value to be the value
//passed in as the function argument.
node *create_node(char *data, char *key);

//Frees every node up in the hash table to avoid memory leaking. Iterates
//through each list in the array and then goes along the list until every index
//has been freed.
void free_hash(hash_table *h);

//Frees up a single node.
void free_node(node *n);

//Performs the Java hash function on a value input
int hash_function(const char *c, hash_table *h);

//Rehashes the hash table to make it double the size, useful for storing more
//elements to reduce the risk of data collisions. Items are removed from old
//table and are all run through hash function on new table size.
void hash_again(hash_table *h);

//Gets the index in the array that the key passed into the function relates to,
//used when inserting items into the hash table.
int get_index(hash_table *h, char *key, int index);

//Gets a specific node from the hash table as there can be more than one node
//in the list, used when inserting items into the hash table
node *get_node(hash_table *h, int list_index, int index);

//Checks to see if the list in the array at index i is empty useful when
//inserting values to see if the key is already in the table.
bool is_list_empty(int i, hash_table *h);

//Checks to see if the hash table is empty, returns true if empty. false if not.
bool is_table_empty(hash_table *h);

//Checks to see if the hash table is full, returns true if full, false if not.
bool is_table_full (hash_table *h);

//Inserts a new value into the hash table, if the key is already in the table
//the value will be inserted in the next index in the list, if the key is not
//already in the table, a new list will be created with the key and the value
//will be inserted in the first index.
void insert(hash_table *h, char *key, char *data);

//Deletes a node from the hash table.
void del(hash_table *h, char *key);
