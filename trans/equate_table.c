/**
 * @file   equate_table.c
 * @author Matheus Vieira Portela
 * @author Lucas de Levy Oliveira
 * @date   18/04/2014
 *
 * @brief  Implements the initialization of the equate directives table
 */

#include "equate_table.h"

/**
 * Init a equate directive table
 * @param equate_table a table pointer to store all directives.
 */
void equate_table_init(hash_table_t *equate_table)
{
    hash_create(equate_table, "Equate directives");
}

/**
 * Add a new equate declaration to the equate directives table.
 * @param equate_table A table pointer to a already initialised hash table.
 * @param label Instruction label string.
 * @param value Corresponding value of the equate.
 */
void equate_table_add(hash_table_t *equate_table, char *label, char *value)
{
    equate_t *equate = malloc(sizeof(equate_t));
    
    strcpy(equate->value, value);
    hash_insert(equate_table, label, equate);
}