/**
 * @file   instructions_table.c
 * @author Matheus Vieira Portela
 * @date   06/04/2014
 *
 * @brief  Implements the initialization of the instructions and opcodes tables
 */

#include "instructions_table.h"

/**
 * Init a instructions table with the hardcoded instructions.
 * @param instructions_table a table pointer to store all instructions.
 */
void instructions_table_init(table_t *instructions_table)
{
    table_init(instructions_table);
    table_add(instructions_table, "ADD", 2);
    table_add(instructions_table, "SUB", 2);
    table_add(instructions_table, "MUL", 2);
    table_add(instructions_table, "DIV", 2);
    table_add(instructions_table, "JMP", 2);
    table_add(instructions_table, "JMPN", 2);
    table_add(instructions_table, "JMPP", 2);
    table_add(instructions_table, "JMPZ", 2);
    table_add(instructions_table, "COPY", 3);
    table_add(instructions_table, "LOAD", 2);
    table_add(instructions_table, "STORE", 2);
    table_add(instructions_table, "INPUT", 2);
    table_add(instructions_table, "OUTPUT", 2);
    table_add(instructions_table, "STOP", 1);
}

/**
 * Init a opcodes table with the hardcoded instructions opcodes.
 * @param opcodes_table a table pointer to store all instructions opcodes.
 */
void opcodes_table_init(table_t *opcodes_table)
{
    table_init(opcodes_table);
    table_add(opcodes_table, "ADD", 0x1);
    table_add(opcodes_table, "SUB", 0x2);
    table_add(opcodes_table, "MUL", 0x3);
    table_add(opcodes_table, "DIV", 0x4);
    table_add(opcodes_table, "JMP", 0x5);
    table_add(opcodes_table, "JMPN", 0x6);
    table_add(opcodes_table, "JMPP", 0x7);
    table_add(opcodes_table, "JMPZ", 0x8);
    table_add(opcodes_table, "COPY", 0x9);
    table_add(opcodes_table, "LOAD", 0xA);
    table_add(opcodes_table, "STORE", 0xB);
    table_add(opcodes_table, "INPUT", 0xC);
    table_add(opcodes_table, "OUTPUT", 0xD);
    table_add(opcodes_table, "STOP", 0xE);
}