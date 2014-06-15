/**
 * @file   symbols_table.c
 * @author Matheus Vieira Portela
 * @author Lucas de Levy Oliveira
 * @date   10/04/2014
 *
 * @brief  Implements the initialization of the symbols tables
 */

#include "symbols_table.h"

/**
 * Init a symbols table
 * @param symbols_table a table pointer to store all the labels.
 */
void symbols_table_init(hash_table_t *symbols_table)
{
    hash_create(symbols_table, "Symbols");
}

/**
 * Add a new label to the symbols table.
 * @param symbols_table a table pointer to an already initialised hash table.
 * @param label instruction label string.
 * @param value position in the memory.
 * @param line_number source code line at which the label was called or defined.
 */
void symbols_table_add(hash_table_t *symbols_table, char *label, int value, int line_number)
{
    symbol_t *symbol = malloc(sizeof(symbol_t));
    
    symbol->value = value;
    symbol->defined = 0;
    symbol->line_number = line_number;
    hash_insert(symbols_table, label, symbol);
}

/**
 * Black magic for checking whether any symbol was not defined yet.
 *
 * The implementation is not trivial but the idea is simple: access each hash table
 * position, access each linked list node and check whether the defined flag is set.
 * If any node has not been defined, return immediately its hash list node, which contains
 * all the necessary info (label and line number). Otherwise, keep looking until no node
 * is left and return NULL in the end.
 *
 * The following diagram shows the relationship between the pointers, since they are
 * strongly coupled.
 *
 * list_node_t [ *data | *next ]
 *                ||
 *                \/   / This is the hash list node pointer to be returned
 *         hash_list_node_t [ key | *data ]
 *                                    ||
 *                                    \/                / This is the defined flag to be checked
 *                                symbol_t [ value | defined | line_number ]
 *
 * @param symbols_table a table pointer to an already initialised hash table.
 * @return the label that is undefined or NULL.
 */
hash_list_node_t* symbols_table_has_undefined(hash_table_t *symbols_table)
{
    int i;
    list_t *list_ptr;
    list_node_t *current_node_ptr;
    
    /* Check each hash table position */
    for (i = 0; i < HASH_TABLE_SIZE; ++i)
    {
        /* Get linked list pointer for the current table position */
        list_ptr = &symbols_table->table[i];
        
        /* Start at the head of the list */
        current_node_ptr = list_ptr->head;
        
        /* Run throughout the list */
        while (current_node_ptr != NULL)
        {
            /* Black magic */
            if (!((symbol_t*)((hash_list_node_t*)current_node_ptr->data)->data)->defined)
                return ((hash_list_node_t*)current_node_ptr->data);
            
            /* Keep looking */
            current_node_ptr = current_node_ptr->next;
        }
    }
    
    return NULL;
}