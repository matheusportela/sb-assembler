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
void directives_table_init(table_t *directives_table)
{
	table_init(directives_table);
	table_add(directives_table, "SPACE", 1);
	table_add(directives_table, "CONST", 1);
}