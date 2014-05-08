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
void instructions_table_init(hash_table_t *instructions_table)
{
    hash_create(instructions_table, "Instructions");
    instructions_table_add(instructions_table, "ADD", 2, ADD_OPCODE);
    instructions_table_add(instructions_table, "SUB", 2, SUB_OPCODE);
    instructions_table_add(instructions_table, "MULT", 2, MULT_OPCODE);
    instructions_table_add(instructions_table, "DIV", 2, DIV_OPCODE);
    instructions_table_add(instructions_table, "JMP", 2, JMP_OPCODE);
    instructions_table_add(instructions_table, "JMPN", 2, JMPN_OPCODE);
    instructions_table_add(instructions_table, "JMPP", 2, JMPP_OPCODE);
    instructions_table_add(instructions_table, "JMPZ", 2, JMPZ_OPCODE);
    instructions_table_add(instructions_table, "COPY", 3, COPY_OPCODE);
    instructions_table_add(instructions_table, "LOAD", 2, LOAD_OPCODE);
    instructions_table_add(instructions_table, "STORE", 2, STORE_OPCODE);
    instructions_table_add(instructions_table, "INPUT", 2, INPUT_OPCODE);
    instructions_table_add(instructions_table, "OUTPUT", 2, OUTPUT_OPCODE);
    instructions_table_add(instructions_table, "STOP", 1, STOP_OPCODE);
}

/**
 * Add a new instruction to the instructions table.
 * @param instructions_table a table pointer to a already initialised hash table.
 * @param label instruction label string.
 * @param size instruction size in memory, in words.
 * @param opcode instruction opcode number.
 */
void instructions_table_add(hash_table_t *instructions_table, char *label, int size,
                            int opcode)
{
    instruction_t *instruction = malloc(sizeof(instruction_t));
    
    instruction->size = size;
    instruction->opcode = opcode;
    hash_insert(instructions_table, label, instruction);
}