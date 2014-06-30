/**
 * @file   asm_opcodes_table.h
 * @author Matheus Vieira Portela
 * @author Lucas de Levy Oliveira
 * @date   06/04/2014
 *
 * @brief  Declares the instructions and the opcodes tables
 */

#ifndef _ASM_OPCODES_TABLE_H_
#define _ASM_OPCODES_TABLE_H_

#include "hash_table.h"

typedef struct
{
    char instruction[7];
    int size;
} asm_opcode_t;

void asm_opcodes_table_init(hash_table_t *asm_opcodes_table);
void asm_opcodes_table_add(hash_table_t *asm_opcodes_table, char *label, char *instruction,
                           int size);

#endif /* _ASM_OPCODES_TABLE_H_ */