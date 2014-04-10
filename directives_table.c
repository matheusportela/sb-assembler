/**
 * @file   directives_table.c
 * @author Matheus Vieira Portela
 * @date   06/04/2014
 *
 * @brief  Implements the initialization of the directives table
 */

#include "directives_table.h"

/**
 * Init a directives table with the hardcoded directives.
 * @param directives_table a table pointer to store all directives.
 */
void directives_table_init(hash_table_t *directives_table)
{
    hash_create(directives_table, "Directives");
    directives_table_add(directives_table, "SPACE", 1);
    directives_table_add(directives_table, "CONST", 1);
}

/**
 * Add a new directive to the directives table.
 * @param directives_table a table pointer to a already initialised hash table.
 * @param label instruction label string.
 * @param size instruction size in memory, in words.
 */
void directives_table_add(hash_table_t *directives_table, char *label, int size)
{
    directive_t *directive = malloc(sizeof(directive_t));
    
    directive->size = size;
    hash_insert(directives_table, label, directive);
}