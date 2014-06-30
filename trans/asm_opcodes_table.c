/**
 * @file   asm_opcodes_table.c
 * @author Matheus Vieira Portela
 * @author Lucas de Levy Oliveira
 * @date   06/04/2014
 *
 * @brief  Implements the initialization of the instructions and opcodes tables
 */

#include "asm_opcodes_table.h"

/**
 * Init a instructions table with the hardcoded instructions.
 * @param asm_opcodes_table a table pointer to store all instructions.
 */
void asm_opcodes_table_init(hash_table_t *asm_opcodes_table)
{
    hash_create(asm_opcodes_table, "ASM Opcodes");
    asm_opcodes_table_add(asm_opcodes_table, "1", "ADD", 2);
    asm_opcodes_table_add(asm_opcodes_table, "2", "SUB", 2);
    asm_opcodes_table_add(asm_opcodes_table, "3", "MULT", 2);
    asm_opcodes_table_add(asm_opcodes_table, "4", "DIV", 2);
    asm_opcodes_table_add(asm_opcodes_table, "5", "JMP", 2);
    asm_opcodes_table_add(asm_opcodes_table, "6", "JMPN", 2);
    asm_opcodes_table_add(asm_opcodes_table, "7", "JMPP", 2);
    asm_opcodes_table_add(asm_opcodes_table, "8", "JMPZ", 2);
    asm_opcodes_table_add(asm_opcodes_table, "9", "COPY", 3);
    asm_opcodes_table_add(asm_opcodes_table, "10", "LOAD", 2);
    asm_opcodes_table_add(asm_opcodes_table, "11", "STORE", 2);
    asm_opcodes_table_add(asm_opcodes_table, "12", "INPUT", 2);
    asm_opcodes_table_add(asm_opcodes_table, "13", "OUTPUT", 2);
    asm_opcodes_table_add(asm_opcodes_table, "14", "STOP", 1);
}

/**
 * Add a new instruction to the instructions table.
 * @param asm_opcodes_table a table pointer to a already initialised hash table.
 * @param label instruction label string.
 * @param size instruction size in memory, in words.
 * @param opcode instruction opcode number.
 */
void asm_opcodes_table_add(hash_table_t *asm_opcodes_table, char *label, char *instruction,
                           int size)
{
    asm_opcode_t *asm_opcode = malloc(sizeof(asm_opcode_t));
    
    strcpy(asm_opcode->instruction, instruction);
    asm_opcode->size = size;
    hash_insert(asm_opcodes_table, label, asm_opcode);
}