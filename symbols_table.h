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
} symbol_t;

void symbols_table_init(hash_table_t *symbols_table);
void symbols_table_add(hash_table_t *symbols_table, char *label, int value);

#endif /* _SYMBOLS_TABLE_H_ */