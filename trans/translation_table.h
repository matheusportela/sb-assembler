/**
 * @file   translation_table.h
 * @author Matheus Vieira Portela
 * @author Lucas de Levy Oliveira
 * @date   06/04/2014
 *
 * @brief  Declares the instructions and the opcodes tables
 */

#ifndef _TRANSLATION_TABLE_H_
#define _TRANSLATION_TABLE_H_

#include "hash_table.h"

typedef struct
{
    char instruction[100];
} translation_t;

void translation_table_init(hash_table_t *translation_table);
void translation_table_add(hash_table_t *translation_table, char *label, char *instruction);

#endif /* _TRANSLATION_TABLE_H_ */