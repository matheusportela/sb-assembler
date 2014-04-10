/**
 * @file   instructions_table.h
 * @author Matheus Vieira Portela
 * @date   06/04/2014
 *
 * @brief  Declares the instructions and the opcodes tables
 */

#ifndef _INSTRUCTIONS_TABLE_H_
#define _INSTRUCTIONS_TABLE_H_

#include "hash_table.h"

typedef struct
{
    int size;
    int opcode;
} instruction_t;

void instructions_table_init(hash_table_t *instructions_table);
void instructions_table_add(hash_table_t *instructions_table, char *label, int size,
                            int opcode);

#endif /* _INSTRUCTIONS_TABLE_H_ */