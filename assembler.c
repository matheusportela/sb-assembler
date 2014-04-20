/**
 * @file   assembler.c
 * @author Matheus Vieira Portela
 * @date   06/04/2014
 *
 * @brief  Two-pass assembler for educational assembly language
 */

#include "assembler.h"

/**
 * First-pass algorithm
 * It checks the declaration of each label, including them to the symbols table, given an
 * error when a label is redefined.
 * Also, it checks if all operations are present in the language, given an error if they
 * do not exist.
 * @param filename the name of the file that is being assembled.
 * @param symbols_table table structure that holds all labels and the address of its
 *                        declaration
 * @param instructions_table structure that holds all instructions and their size
 * @param directives_table structure that holds all directives
 * @return The size of the assembled program
 */
int assembler_first_pass(char *filename,
                         hash_table_t *symbols_table,
                         hash_table_t *instructions_table,
                         hash_table_t *directives_table)
{
    FILE *fp;
    char line_buffer[FILE_LINE_LENGTH];
    element_t elements;
    instruction_t *instruction_ptr;
    directive_t *directive_ptr;
    int position_counter = 0;
    
    fp = file_open(filename, "r");
    
    while(file_read_line(fp, line_buffer) != FILE_FINISHED)
    {
        scan_line_elements(&elements, line_buffer);
        
        /*
         * Looks for label in the symbols table.
         * Gives error if it was already defined
         */
        if (element_has_label(&elements))
        {
            if (hash_search(symbols_table, elements.label))
            {
                fprintf(stderr, "ERROR: Redefined label: %s\n", elements.label);
                exit(-1);
            }
    
            symbols_table_add(symbols_table, elements.label, position_counter);
        }
        
        /*
         * Looks for operation in the operations table or directives table.
         * Gives error if it is not defined in the language or as a proper directive
         */
        instruction_ptr = hash_search(instructions_table, elements.operation);
        if (instruction_ptr)
        {
            position_counter += instruction_ptr->size;
        }
        else
        {
            directive_ptr = hash_search(directives_table, elements.operation);
            if (directive_ptr)
            {
                /*
                 * TODO: This should execute the subroutine of the directive
                 * Update as soon as the directives table are implemented
                 */
                position_counter += directive_ptr->function();
            }
            else
            {
                fprintf(stderr, "ERROR: Operation %s not valid\n", elements.operation);
                exit(-1);
            }
        }
        
        element_clear(&elements); /* So as one line does not interfere to the other */
    }
    
    file_close(fp);
    
    return position_counter;
}

/**
 * Second-pass algorithm
 * It checks the consistency of each operand, i.e., if they are labels, should be
 * previously defined. Otherwise, it raises an error message.
 * Also, it checks if all operations are valid and have the right number of operands.
 * Finally, it generates the compiled program in object code.
 * @param filename the name of the file that is being assembled.
 * @param symbols_table table structure that holds all labels and the address of its
 *                        declaration
 * @param instructions_table structure that holds all instructions and their size
 * @param opcodes_table structure that holds all instructions opcodes
 * @param directives_table structure that holds all directives
 * @return The size of the assembled program
 */
void assembler_second_pass(char *filename,
                           hash_table_t *symbols_table,
                           hash_table_t *instructions_table,
                           hash_table_t *directives_table,
                           const int program_size)
{
    FILE *fp;
    char line_buffer[FILE_LINE_LENGTH];
    element_t elements;
    instruction_t *instruction_ptr;
    directive_t *directive_ptr;
    symbol_t *symbol_ptr;
    int i;
    int position_counter = 0;
    int compiled_program[program_size];
    int instruction_size;
    
    for (i = 0; i < program_size; ++i)
        compiled_program[i] = 0;
    
    fp = file_open(filename, "r");
    
    while(file_read_line(fp, line_buffer) != FILE_FINISHED)
    {
        scan_line_elements(&elements, line_buffer);
        
        /*
         * Looks for the operands in the symbols table
         * Gives error if it was not previously defined
         * Numbers are not symbols, so they should be ignored
         */
        if (element_has_operand1(&elements)
            && (!hash_search(symbols_table, elements.operand1))
            && (!is_number(elements.operand1)))
        {
            fprintf(stderr, "ERROR: Undefined operand: %s\n", elements.operand1);
            exit(-1);
        }
        
        if (element_has_operand2(&elements)
            && (!hash_search(symbols_table, elements.operand2))
            && (!is_number(elements.operand2)))
        {
            fprintf(stderr, "ERROR: Undefined operand: %s\n", elements.operand2);
            exit(-1);
        }
        
        /*
         * Looks for operation in the operations table or directives table
         * Gives error if it has a wrong number of operands or as a proper directive
         */
        instruction_ptr = hash_search(instructions_table, elements.operation);
        if (instruction_ptr)
        {
            instruction_size = instruction_ptr->size;
        
            if ((instruction_size - 1) == element_count_operands(&elements))
            {
                /*
                 * Generates object-code OPERATION OPERAND1 OPERAND2
                 */
                compiled_program[position_counter] = instruction_ptr->opcode;
                
                if(element_has_operand1(&elements))
                {
                    symbol_ptr = hash_search(symbols_table, elements.operand1);
                    compiled_program[position_counter + 1] = symbol_ptr->value;
                }
                
                if(element_has_operand2(&elements))
                {
                    symbol_ptr = hash_search(symbols_table, elements.operand2);
                    compiled_program[position_counter + 2] = symbol_ptr->value;
                }
            }
            else
            {
                fprintf(stderr, "ERROR: Wrong number of operands "
                                "(it is %d but should be %d)\n",
                        element_count_operands(&elements),
                        (instruction_ptr->size - 1));
                exit(-1);
            }
            
            position_counter += instruction_size;
        }
        else
        {
            directive_ptr = hash_search(directives_table, elements.operation);
            if (directive_ptr)
            {
                instruction_size = directive_ptr->size;
                
                /*
                 * TODO: This should execute the subroutine of the directive
                 * Update as soon as the directives table are implemented
                 */
                if(element_has_operand1(&elements))
                    compiled_program[position_counter] = atoi(elements.operand1);
                
                position_counter += directive_ptr->function();
            }
            else
            {
                fprintf(stderr, "ERROR: Operation %s not valid\n", elements.operation);
                exit(-1);
            }
        }
        
        element_clear(&elements); /* So as one line does not interfere to the other */
        
        /* Print the object code line for visualization purposes */
        printf("(addr. %d)", (position_counter - instruction_size));
        for (i = instruction_size; i > 0; --i)
            printf(" %d", compiled_program[position_counter - i]);
        printf("\n");
    }
    
    file_close(fp);
}