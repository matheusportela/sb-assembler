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
    directives_table_add(directives_table, "SPACE", 1, directives_run_space);
    directives_table_add(directives_table, "CONST", 1, directives_run_const);
}

/**
 * Add a new directive to the directives table.
 * @param directives_table a table pointer to a already initialised hash table.
 * @param label instruction label string.
 * @param size instruction size in memory, in words.
 */
void directives_table_add(hash_table_t *directives_table, char *label, int size, int (*function)(void))
{
    directive_t *directive = malloc(sizeof(directive_t));
    
    directive->size = size;
    directive->function = function;
    hash_insert(directives_table, label, directive);
}

/**
 * Execute the SPACE directive.
 * @return the amount of space in memory occupied by the directive
 */
int directives_run_space()
{
    return 1; /* space only occupies one space in memory */
}

/**
 * Execute the CONST directive.
 * @return the amount of space in memory occupied by the directive
 */
int directives_run_const()
{
    return 1; /* const only occupies one space in memory */
}