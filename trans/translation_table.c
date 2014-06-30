/**
 * @file   translation_table.c
 * @author Matheus Vieira Portela
 * @author Lucas de Levy Oliveira
 * @date   06/04/2014
 *
 * @brief  Implements the initialization of the instructions and opcodes tables
 */

#include "translation_table.h"

/**
 * Init a instructions table with the hardcoded instructions.
 * @param translation_table a table pointer to store all instructions.
 */
void translation_table_init(hash_table_t *translation_table)
{
    hash_create(translation_table, "Translation");
    translation_table_add(translation_table, "ADD", "add");
    translation_table_add(translation_table, "SUB", "sub");
    translation_table_add(translation_table, "MULT", "mul");
    translation_table_add(translation_table, "DIV", "div");
    translation_table_add(translation_table, "JMP", "jmp");
    translation_table_add(translation_table, "JMPN", "NO INSTRUCTION");
    translation_table_add(translation_table, "JMPP", "NO INSTRUCTION");
    translation_table_add(translation_table, "JMPZ", "NO INSTRUCTION");
    translation_table_add(translation_table, "COPY", "mov");
    translation_table_add(translation_table, "LOAD", "push");
    translation_table_add(translation_table, "STORE", "pop");
    translation_table_add(translation_table, "INPUT", "call EscreverInteiro");
    translation_table_add(translation_table, "OUTPUT", "call LerInteiro");
    translation_table_add(translation_table, "STOP", "call Stop");
}

/**
 * Add a new instruction to the instructions table.
 * @param translation_table a table pointer to a already initialised hash table.
 * @param label instruction label string.
 * @param size instruction size in memory, in words.
 * @param opcode instruction opcode number.
 */
void translation_table_add(hash_table_t *translation_table, char *label, char *instruction)
{
    translation_t *translation = malloc(sizeof(translation_t));
    
    strcpy(translation->instruction, instruction);
    hash_insert(translation_table, label, translation);
}