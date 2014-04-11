/**
 * @file   hash_table.c
 * @author Matheus Vieira Portela
 * @date   10/04/2014
 *
 * @brief  Implements hash table functions
 *
 * A hash table have an array of linked lists to hold data.
 */
 
#include "hash_table.h"

/**
 * Initialise all hash table linked lists, one for each hash table position.
 * @param hash_table pointer for the previously allocated hash table.
 */
void hash_create(hash_table_t *hash_table, char *name)
{
    int i;
    
    for (i = 0; i < HASH_TABLE_SIZE; ++i)
    {
        list_create(&hash_table->table[i], sizeof(hash_list_node_t),
                    (void*)hash_node_compare, (void*)hash_node_print);
    }
    
    hash_table->name = name;
}

/**
 * Free all hash table linked lists.
 * @param hash_table pointer for the previously initialised hash table.
 */
void hash_destroy(hash_table_t *hash_table)
{
    int i;
    for (i = 0; i < HASH_TABLE_SIZE; ++i)
        list_destroy(&hash_table->table[i]);
}

/**
 * Look for a node in the hash table based on the given key.
 * @param hash_table pointer for the previously initialised hash table.
 * @param key string pointer that defines the key to be searched.
 * @return the node data if there is a node with the given key, otherwise, NULL.
 */
void* hash_search(hash_table_t *hash_table, char *key)
{
    unsigned int hash_index;
    hash_list_node_t *hash_list_node;
    
    hash_index = hash_function(key);
    hash_list_node = list_search(&hash_table->table[hash_index], key);
    
    if (hash_list_node != NULL)
        return hash_list_node->data;
    
    return NULL;
}

/**
 * Insert some data to the hash table, attached to a key.
 * @param hash_table pointer for the previously initialised hash table.
 * @param key string pointer that defines the key to be searched.
 * @param data pointer to the previously allocated data.
 */
void hash_insert(hash_table_t *hash_table, char *key, void *data)
{
    unsigned int hash_index = hash_function(key);
    hash_list_node_t *hash_list_node = malloc(sizeof(hash_list_node_t));
    
    strcpy(hash_list_node->key, key);
    hash_list_node->data = data;

    list_append(&hash_table->table[hash_index], hash_list_node);
}

/**
 * Calculate the hash of a given key.
 * @param key string pointer that defines the key to be hashed.
 * @return the hash value for the key.
 */
unsigned int hash_function(char *key)
{
    unsigned int hash_value = 0;
    int i;
    
    /*
     * Multiply the old hash by 31 and add the current character
     */
    for (i = 0; key[i] != '\0'; ++i)
        hash_value = (unsigned int)key[i] + (hash_value << 5) - hash_value;
        
    /*
     * Bound the hash value to the hash table size by using the mod operand
     */
    return (hash_value % HASH_TABLE_SIZE);
}

/**
 * Print all keys contained in the hash table.
 * @param hash_table pointer for the previously initialised hash table.
 */
void hash_print(hash_table_t *hash_table)
{
    int i;

    if (hash_table->name != NULL)
        printf("=== Hash table: %s ===\n", hash_table->name);
    else
        printf("=== Hash table: ===\n");
    
    for (i = 0; i < HASH_TABLE_SIZE; ++i)
    {
        printf("%d:\n", i);
        list_print(&hash_table->table[i]);
    }
    printf("=====\n");
}

/**
 * Compare a hash node list key to a given key.
 * @param node pointer for the previously initialised hash list node.
 * @return 1 if the node key and the given key are the same, 0 otherwise.
 */
int hash_node_compare(hash_list_node_t *node, char *key)
{
    if (strcmp(node->key, key) == 0)
        return 1;
        
    return 0;
}

/**
 * Print a hash node key.
 * @param node pointer for the previously initialised hash list node.
 */
void hash_node_print(hash_list_node_t *node)
{
    printf("  Key: %s\n", node->key);
}