/**
 * @file   instructions_table.h
 * @author Matheus Vieira Portela
 * @date   06/04/2014
 *
 * @brief  Declares the instructions and the opcodes tables
 */

#ifndef _INSTRUCTIONS_TABLE_H_
#define _INSTRUCTIONS_TABLE_H_

#include "table.h"

void instructions_table_init(table_t *instructions_table);
void opcodes_table_init(table_t *opcodes_table);

#endif /* _INSTRUCTIONS_TABLE_H_ */