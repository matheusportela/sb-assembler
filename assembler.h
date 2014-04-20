/**
 * @file   assembler.h
 * @author Matheus Vieira Portela
 * @date   06/04/2014
 *
 * @brief  Two-pass assembler for educational assembly language
 *
 * This assembler was built to assemble the language used in the Systems Software course
 * offered at the University of Brasilia, Brazil, first semester of 2014.
 * 
 * The language assumes the existence of a theoretical computer, which has only one 16-bit
 * register (named accumulator, or ACC), one 16-bit program counter (PC), 16-bit addressed
 * memory with 216 words, and instructions in one of the following formats:
 * Format 1: Opcode
 * Format 2: Opcode Address
 * Format 3: Opcode Address1 Address2
 * 
 * These are the available instructions, with their opcode, size (in words), and action:
 * Name     | Opcode    | Size  | Action
 * ADD      | 01        | 2     | ACC <- ACC + mem(OP)
 * SUB      | 02        | 2     | ACC <- ACC - mem(OP)
 * MUL      | 03        | 2     | ACC <- ACC * mem(OP)
 * DIV      | 04        | 2     | ACC <- ACC / mem(OP)
 * JMP      | 05        | 2     | PC <- OP
 * JMPN     | 06        | 2     | If ACC < 0 then PC <- OP
 * JMPP     | 07        | 2     | If ACC > 0 then PC <- OP
 * JMPZ     | 08        | 2     | If ACC = 0 then PC <- OP
 * COPY     | 09        | 3     | mem(OP2) <- mem(OP1)
 * LOAD     | 10        | 2     | ACC <- mem(OP)
 * STORE    | 11        | 2     | mem(OP) <- ACC
 * INPUT    | 12        | 2     | mem(OP) <- input
 * OUTPUT   | 13        | 2     | output <- mem(OP)
 * STOP     | 14        | 1     | Stops execution
 * 
 * The following directives are also available, with its parameters in parenthesis when
 * necessary:
 * SPACE: Alloc space in memory (only one word)
 * CONST (NUM): Write NUM to the memory in the address occupied by the directive
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "object_file.h"
#include "elements.h"
#include "scanner.h"
#include "directives_table.h"
#include "instructions_table.h"
#include "symbols_table.h"
#include "linked_list.h"
#include "hash_table.h"
#include "preprocessing.h"

int assembler_first_pass(char *filename,
                         hash_table_t *symbols_table,
                         hash_table_t *instructions_table,
                         hash_table_t *directives_table);
void assembler_second_pass(char *filename,
                           hash_table_t *symbols_table,
                           hash_table_t *instructions_table,
                           hash_table_t *directives_table,
                           const int program_size);