/**
 * @file   translator.c
 * @author Matheus Vieira Portela
 * @author Lucas de Levy Oliveira
 * @date   29/05/2014
 *
 * @brief  One-pass assembler for educational assembly language.
 *
 * The assembler is implemented as a one-pass assembly, meaning that it must assembly the
 * whole source code reading only it one time.
 *
 * The choice of a one-pass assembly implies in dealing with forward declarations
 * explicitly.
 *
 * The assembler uses four hash tables:
 * - Symbols table: stores every used symbol and the location in the object code of its 
 *                  last appearance.
 * - Instructions table: stores every valid instruction and its opcode.
 * - Directives table: stores every valid directive.
 * - Constants table: stores every label that points to a defined as a constant memory
 *                    address.
 */

#include "translator.h"
#include "object_file.h"

/**
 * Assemble a given source code input and writes the object file to the output file. It
 * first parses each line from the source and analyse its label, operation and operands.
 * Errors happen when an operation cannot be identified as an instruction nor as a
 * directive. Undefined labels and writing to constant memory addresses errors can only be
 * evaluated after parsing the whole source code.
 * @param input Source code file name.
 * @param output Object file name.
 */
void translate(char *input, char *output)
{
    printf("===== Translating =====\n");
    object_file_t obj;
    object_file_read(input, &obj);
}