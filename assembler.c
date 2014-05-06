/**
 * @file   assembler.c
 * @author Matheus Vieira Portela
 * @date   06/04/2014
 *
 * @brief  One-pass assembler for educational assembly language
 */

#include "assembler.h"

/**
 * Evaluate a given label.
 * @param label Given label for evaluation.
 * @param symbols_table Tables containing all symbols defined so far.
 * @param object_file_ptr Pointer to the object file.
 * @param line_number Number of the current line in the source file.
 */
void evaluate_label(char *label, hash_table_t *symbols_table,
                    object_file_t *object_file_ptr, int line_number)
{
    symbol_t *symbol_ptr;
    int previous_position;
    int aux_position;

    /* Label analysis */
    if (!is_valid_label(label))
        error_at_line(ERROR_LEXICAL, line_number, "\"%s\" is not a valid label name",
                      label);
    
    /* Label not in the table yet */
    if (!(symbol_ptr = hash_search(symbols_table, label)))
    {
        symbols_table_add(symbols_table, label, object_file_ptr->size, line_number);
        symbol_ptr = hash_search(symbols_table, label);
        symbol_ptr->defined = 1;
    }
    else
    {
        /* Label already in the table and defined */
        if (symbol_ptr->defined)
        {
            error_at_line(ERROR_SEMANTIC, line_number, "Redefined label \"%s\"", label);
        }
        /* Label already in the table but being defined now */
        else
        {
            symbol_ptr->defined = 1;
        
            previous_position = symbol_ptr->value;
            symbol_ptr->value = object_file_ptr->size;
        
            /* Replace previous definitions */
            while (previous_position != -1)
            {
                aux_position = object_file_get(*object_file_ptr, previous_position);
                object_file_insert(object_file_ptr, previous_position, symbol_ptr->value);
                previous_position = aux_position;
            }
        }
    }
}

/**
 * Check whether a instruction is valid and evaluate
 * @return the instruction size if it is a valid instruction, 0 otherwise
 */
int evaluate_instruction(char *instruction, char *operand1, char *operand2,
                         hash_table_t *instructions_table, section_t section,
                         int line_number, object_file_t *object_file, write_t *write_list,
                         int *write_num)
{
    instruction_t *instruction_ptr;

    if ((instruction_ptr = hash_search(instructions_table, instruction)))
    {
        if (section == SECTION_DATA)
            error_at_line(ERROR_SEMANTIC, line_number, "Using instruction \"%s\" in "
                          "the data section", instruction);
    
        object_file_add(object_file, instruction_ptr->opcode);
        
        /* For later checking whether writing in const memory */
        if ((strcmp(instruction, "STORE") == 0) || (strcmp(instruction, "INPUT") == 0))
        {
            strcpy(write_list[*write_num].label, operand1);
            write_list[*write_num].line_number = line_number;
            ++(*write_num);
        }
        else if (strcmp(instruction, "COPY") == 0)
        {
            strcpy(write_list[*write_num].label, operand2);
            write_list[*write_num].line_number = line_number;
            ++(*write_num);
        }
    
        return instruction_ptr->size;
    }
    
    return 0;
}

void evaluate_operand1(char *instruction, char *operand1, int instruction_size,
                       hash_table_t *symbols_table, object_file_t *object_file,
                       int line_number)
{
    symbol_t *symbol_ptr;
    
    if (instruction_size == 1)
        error_at_line(ERROR_SYNTACTIC, line_number, "Instruction \"%s\" does "
                      "not accept arguments", instruction);

    if (!(symbol_ptr = hash_search(symbols_table, operand1)))
    {
        symbols_table_add(symbols_table, operand1, object_file->size, line_number);
        object_file_add(object_file, -1);
    }
    else
    {
        if (symbol_ptr->defined)
        {
            object_file_add(object_file, symbol_ptr->value);
        }
        else
        {
            object_file_add(object_file, symbol_ptr->value);
            symbol_ptr->value = object_file->size - 1;
        }
    }
}

void evaluate_operand2(char *instruction, char *operand2, int instruction_size,
                       hash_table_t *symbols_table, object_file_t *object_file,
                       int line_number)
{
    symbol_t *symbol_ptr;
    
    if (instruction_size == 2)
        error_at_line(ERROR_SYNTACTIC, line_number, "Instruction \"%s\" only "
                      "accepts one argument", instruction);
    
    if (!(symbol_ptr = hash_search(symbols_table, operand2)))
    {
        symbols_table_add(symbols_table, operand2, object_file->size, line_number);
        object_file_add(object_file, -1);
    }
    else
    {
        if (symbol_ptr->defined)
        {
            object_file_add(object_file, symbol_ptr->value);
        }
        else
        {
            object_file_add(object_file, symbol_ptr->value);
            symbol_ptr->value = object_file->size - 1;
        }
    }
}

int evaluate_directive(char *directive, element_t *elements,
                       hash_table_t *directives_table, hash_table_t *constants_table,
                       section_t *section, int *is_data_section_defined,
                       int *is_text_section_defined, int line_number,
                       object_file_t *object_file)
{
    directive_t *directive_ptr;
    const_t *constant;
    
    /* Generate code for directive */
    if ((directive_ptr = hash_search(directives_table, directive)))
    {
        if (strcmp(directive, "CONST") == 0)
        {
            if ((*section) == SECTION_TEXT)
                error_at_line(ERROR_SEMANTIC, line_number, "Using directive \"%s\" "
                              "int the data section", directive);
        
            if (element_has_operand1(elements))
                object_file_add(object_file, strtol(elements->operand1, NULL, 0));
            else
                error_at_line(ERROR_SYNTACTIC, line_number, "CONST directive requires "
                              "one argument");
            
            if (element_has_operand2(elements))
                error_at_line(ERROR_SYNTACTIC, line_number, "CONST directive accepts "
                              "only one argument");
            
            constant = malloc(sizeof(const_t));
            hash_insert(constants_table, elements->label, constant);
        }
        else if (strcmp(directive, "SPACE") == 0)
        {
            if ((*section) == SECTION_TEXT)
                error_at_line(ERROR_SEMANTIC, line_number, "Using directive \"%s\" "
                              "int the data section", directive);
        
            object_file_add(object_file, 0); /* Initializing SPACE with zero value */
        }
        else if (strcmp(directive, "SECTION") == 0)
        {
            if (strcmp(elements->operand1, "DATA") == 0)
            {
                if ((*is_data_section_defined) && (*is_text_section_defined))
                    error_at_line(ERROR_SEMANTIC, line_number, "Section \"%s\" is "
                                  "already defined", elements->operand1);
                
                *is_data_section_defined = 1;
                *section = SECTION_DATA;
            }
            else if (strcmp(elements->operand1, "TEXT") == 0)
            {
                if ((*is_data_section_defined) && (*is_text_section_defined))
                    error_at_line(ERROR_SEMANTIC, line_number, "Section \"%s\" is "
                                  "already defined", elements->operand1);
                
                *is_text_section_defined = 1;
                *section = SECTION_TEXT;
            }
            else
            {
                error_at_line(ERROR_SYNTACTIC, line_number, "Unknown section directive "
                              "\"%s\"", elements->operand1);
            }
            
            if (element_has_operand2(elements))
                error_at_line(ERROR_SYNTACTIC, line_number, "SECTION directive accepts "
                              "only one argument");
        }
        
        return 1;
    }
    
    return 0;
}

void check_undefined_labels(hash_table_t *symbols_table)
{
    hash_list_node_t *hash_list_node_ptr;
    char *label_ptr;
    int line_number;
    
    if ((hash_list_node_ptr = symbols_table_has_undefined(symbols_table)))
    {
        label_ptr = hash_list_node_ptr->key;
        line_number = ((symbol_t*)hash_list_node_ptr->data)->line_number;
        error_at_line(ERROR_SEMANTIC, line_number, "Undefined label \"%s\"", label_ptr);
    }
}

void check_writing_at_const(hash_table_t *constants_table, write_t *write_list, int write_num)
{
    int i;
    const_t *constant;
    
    for (i = 0; i < write_num; ++i)
    {
        if ((constant = hash_search(constants_table, write_list[i].label)))
        {
            error_at_line(ERROR_MEMORY, write_list[i].line_number, "Cannot write to the "
                          "constant label %s", write_list[i].label);
        }
    }
}

void assemble(char *input, char *output)
{
    FILE *fp;
    hash_table_t symbols_table;
    hash_table_t instructions_table;
    hash_table_t directives_table;
    hash_table_t constants_table;
    object_file_t object_file;
    element_t elements;
    char line_buffer[FILE_LINE_LENGTH];
    section_t section = SECTION_UNKNOWN;
    int line_number = 0;
    int instruction_size;
    int is_instruction = 0;
    int is_directive = 0;
    int is_data_section_defined = 0;
    int is_text_section_defined = 0;
    
    /* Used for constant memory writing checking */
    write_t write_list[100];
    int write_num = 0;
    hash_create(&constants_table, "Constants");
    
    /* Initializing */
    init_tables(&symbols_table, &instructions_table, &directives_table);
    object_file_init(&object_file);
    element_init(&elements); /* Avoid garbage values at the label field */
    fp = file_open(input, "r");
    
    /* Assembling */
    printf("===== Assembling =====\n");
    
    printf("=== Parsing ===\n");
    while (file_read_line(fp, line_buffer) != FILE_FINISHED)
    {
        is_instruction = 0;
        is_directive = 0;
        line_number += 1;
        
        /* Scanning */
        scan_line_elements(&elements, line_buffer);
        printf("%15.15s | %15.15s | %15.15s | %15.15s\n", elements.label,
               elements.operation, elements.operand1, elements.operand2);
        
        /* Label analysis */
        if (element_has_label(&elements))
            evaluate_label(elements.label, &symbols_table, &object_file, line_number);
        
        /* Check operation */
        if (element_has_operation(&elements))
        {
            /* Generate code for instruction */
            instruction_size = evaluate_instruction(elements.operation, elements.operand1,
                                                    elements.operand2,
                                                    &instructions_table,
                                                    section, line_number, &object_file,
                                                    write_list, &write_num);
                  
            if (instruction_size > 0)
            {
                is_instruction = 1;
        
                /* Generate code for labels in operands */
                if (element_has_operand1(&elements))
                    evaluate_operand1(elements.operation, elements.operand1,
                                      instruction_size, &symbols_table, &object_file,
                                      line_number);
                
                if (element_has_operand2(&elements))
                    evaluate_operand2(elements.operation, elements.operand2,
                                      instruction_size, &symbols_table, &object_file,
                                      line_number);
            }
            /* Generate code for directive */
            else
            {
                is_directive = evaluate_directive(elements.operation, &elements,
                                                  &directives_table, 
                                                  &constants_table,
                                                  &section,
                                                  &is_data_section_defined,
                                                  &is_text_section_defined,
                                                  line_number, &object_file);
            }
        }   
        
        /* Check for invalid instructions or directives */
        if ((element_has_operation(&elements)) && (!is_instruction) && (!is_directive))
            error_at_line(ERROR_LEXICAL, line_number, "\"%s\" is not a valid instruction "
                          "or directive", elements.operation);
        
        element_clear(&elements); /* So as one line does not interfere to the other */
    }
    
    /* Check for undefined labels */
    check_undefined_labels(&symbols_table);
    
    /* Check for writing in constant memory addresses */
    check_writing_at_const(&constants_table, write_list, write_num);
    
    /* Printing */
    object_file_print(object_file);
    
    /* Writing */
    object_file_write(output, object_file);
    
    /* Finishing */
    object_file_destroy(&object_file);
    destroy_tables(&symbols_table, &instructions_table, &directives_table);
    file_close(fp);
}

void init_tables(hash_table_t *symbols_table, hash_table_t *instructions_table,
                 hash_table_t *directives_table)
{
    symbols_table_init(symbols_table);
    instructions_table_init(instructions_table);
    directives_table_init(directives_table);
}

void destroy_tables(hash_table_t *symbols_table, hash_table_t *instructions_table,
                    hash_table_t *directives_table)
{
    hash_destroy(symbols_table);
    hash_destroy(instructions_table);
    hash_destroy(directives_table);
}