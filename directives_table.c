#include "directives_table.h"

void directives_table_init(table_t *directives_table)
{
	table_init(directives_table);
	table_add(directives_table, "SPACE", 1);
	table_add(directives_table, "CONST", 1);
}