/**
 * @file   equate_table.h
 * @author Matheus Vieira Portela
 * @date   18/04/2014
 *
 * @brief  Declares the equate directives table
 */

#ifndef _EQU_TABLE_H_
#define _EQU_TABLE_H_

#include "hash_table.h"

#define EQUATE_VALUE_SIZE 20

typedef struct
{
    char value[EQUATE_VALUE_SIZE];
} equate_t;

void equate_table_init(hash_table_t *equate_table);
void equate_table_add(hash_table_t *equate_table, char *label, char *value);

#endif /* _EQU_TABLE_H_ */