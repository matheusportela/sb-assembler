/**
 * @file   symbols_table.c
 * @author Matheus Vieira Portela
 * @date   10/04/2014
 *
 * @brief  Implements the initialization of the symbols tables
 */

#include "symbols_table.h"

/**
 * Init a symbols table
 * @param symbols_table a table pointer to store all instructions.
 */
void symbols_table_init(hash_table_t *symbols_table)
{
    hash_create(symbols_table, "Symbols");
}

/**
 * Add a new instruction to the instructions table.
 * @param instructions_table a table pointer to a already initialised hash table.
 * @param label instruction label string.
 * @param size instruction size in memory, in words.
 * @param opcode instruction opcode number.
 */
void symbols_table_add(hash_table_t *symbols_table, char *label, int value)
{
    symbol_t *symbol = malloc(sizeof(symbol_t));
    
    symbol->value = value;
    hash_insert(symbols_table, label, symbol);
}