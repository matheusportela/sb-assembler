/**
 * @file   symbols_table.h
 * @author Matheus Vieira Portela
 * @date   10/04/2014
 *
 * @brief  Declares the symbols table
 */

#ifndef _SYMBOLS_TABLE_H_
#define _SYMBOLS_TABLE_H_

#include "hash_table.h"

typedef struct
{
    int value;
    int defined;
    int offset;
    int line_number;
} symbol_t;

void symbols_table_init(hash_table_t *symbols_table);
void symbols_table_add(hash_table_t *symbols_table, char *label, int value, int line_number);
hash_list_node_t* symbols_table_has_undefined(hash_table_t *symbols_table);

#endif /* _SYMBOLS_TABLE_H_ */