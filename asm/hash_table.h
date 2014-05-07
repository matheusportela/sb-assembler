/**
 * @file   hash_table.h
 * @author Matheus Vieira Portela
 * @date   10/04/2014
 *
 * @brief  Declares hash table node struct, hash table struct and hash table functions
 *
 * Hash tables can be used to store and find elements that respect the node struct.
 * Data are stored in linked lists.
 *
 * Following implementation suggested at:
 * http://www.sparknotes.com/cs/searching/hashtables/section3.rhtml
 *
 * Example usage
 *
    typedef struct
    {
        int x;
        int y;
        char z;
    } hash_data_t;

    int main()
    {
        hash_table_t hash_table;
        hash_data_t a, b, c;
    
        a.x = 1;
        a.y = 2;
        a.z = 'a';
    
        b.x = 3;
        b.y = 4;
        b.z = 'b';
    
        c.x = 5;
        c.y = 6;
        c.z = 'c';
    
    
        hash_create(&hash_table, "Family names");
        hash_print(&hash_table);
    
        hash_insert(&hash_table, "Matheus", &a);
        hash_insert(&hash_table, "Marcelo", &b);
        hash_insert(&hash_table, "Beatriz", &c);
        hash_insert(&hash_table, "Maria", &c);
    
        hash_print(&hash_table);
    
        hash_data_t *data = hash_search(&hash_table, "Marcelo");
    
        if (data != NULL)
            printf("x: %d, y: %d, z: %c\n", data->x, data->y, data->z);
        else
            printf("No data found with key Marcelo\n");
    
        hash_destroy(&hash_table);
    
        return 0;
    }
 */

#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

#include <stdio.h>
#include "linked_list.h"

#define HASH_TABLE_SIZE 10
#define HASH_TABLE_KEY_SIZE 100
#define HASH_TABLE_NAME_SIZE 100

typedef struct
{
    char key[HASH_TABLE_KEY_SIZE];
    void *data;
} hash_list_node_t;

typedef struct
{
    int size;
    char *name;
    list_t table[HASH_TABLE_SIZE];
} hash_table_t;

void hash_create(hash_table_t *hash_table, char *name);
void hash_destroy(hash_table_t *hash_table);
unsigned int hash_function(char *key);
void hash_insert(hash_table_t *hash_table, char *key, void *data);
void* hash_search(hash_table_t *hash_table, char *key);
void hash_print(hash_table_t *hash_table);

int hash_node_compare(hash_list_node_t *node, char *key);
void hash_node_print(hash_list_node_t *node);

#endif /* _HASH_TABLE_H_ */