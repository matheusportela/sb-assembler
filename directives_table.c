/**
 * @file   directives_table.c
 * @author Matheus Vieira Portela
 * @date   06/04/2014
 *
 * @brief  Implements the initialization of the directives table
 */

#include "directives_table.h"

void directives_table_init(table_t *directives_table)
{
	table_init(directives_table);
	table_add(directives_table, "SPACE", 1);
	table_add(directives_table, "CONST", 1);
}