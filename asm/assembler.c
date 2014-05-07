/**
 * @file   assembler.c
 * @author Matheus Vieira Portela
 * @date   06/04/2014
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

#include "assembler.h"

int write_compare(write_t *data, char *label)
{
    if (strcmp(data->label, label) == 0)
        return 1;
    return 0;
}

/**
 * Assemble a given source code input and writes the object file to the output file. It
 * first parses each line from the source and analyse its label, operation and operands.
 * Errors happen when an operation cannot be identified as an instruction nor as a
 * directive. Undefined labels and writing to constant memory addresses errors can only be
 * evaluated after parsing the whole source code.
 * @param input Source code file name.
 * @param output Object file name.
 */
void assemble(char *input, char *output)
{
    /* Source code file */
    FILE *fp;
    
    /* Tables */
    hash_table_t symbols_table;
    hash_table_t instructions_table;
    hash_table_t directives_table;
    hash_table_t constants_table;
    
    /* Object file */
    object_file_t object_file;
    
    /* Parse elements from the source code line */
    element_t elements;
    char line_buffer[FILE_LINE_LENGTH];
    int instruction_size;
    int is_instruction = 0;
    int is_directive = 0;
    int line_number = 0;
    
    /* Current section (either text or data) */
    section_t section = SECTION_UNKNOWN;
    
    /* Used as flags for definition of the sections */
    int is_data_section_defined = 0;
    int is_text_section_defined = 0;
    
    /* Used for writing at constant memory checking */
    list_t write_list;
    int write_num = 0;
    
    list_create(&write_list, sizeof(write_t), (void*)write_compare, NULL);
    
    /* Initializing */
    init_tables(&symbols_table, &instructions_table, &directives_table, &constants_table);
    object_file_init(&object_file);
    element_init(&elements); /* Avoid garbage values at the label field by explicitly
                                initialising */
    fp = file_open(input, "r");
    
    /* Assembling */
    printf("===== Assembling =====\n");
    
    while (file_read_line(fp, line_buffer) != FILE_FINISHED)
    {
        /* Reset variables */
        is_instruction = 0;
        is_directive = 0;
        
        /* Update line number */
        line_number += 1;
        
        /* Scanning */
        scan_line_elements(&elements, line_buffer);
        printf("%15.15s | %15.15s | %15.15s | %15.15s\n", elements.label,
               elements.operation, elements.operand1, elements.operand2);
        
        /* Label analysis */
        if (element_has_label(&elements))
            evaluate_label(&elements, &symbols_table, &object_file, line_number);
        
        /* Check operation field (which can be either an instruction or a directive) */
        if (element_has_operation(&elements))
        {
            /* Generate code for instruction */
            instruction_size = evaluate_instruction(&elements,
                                                    &instructions_table,
                                                    section, line_number, &object_file,
                                                    &write_list, &write_num);
                  
            if (instruction_size > 0) /* Detected as a instruction */
            {
                is_instruction = 1;
        
                /* Generate code for labels in operands */
                if (element_has_operand1(&elements))
                    evaluate_operand1(&elements,
                                      instruction_size, &symbols_table, &object_file,
                                      line_number);
                
                if (element_has_operand2(&elements))
                    evaluate_operand2(&elements,
                                      instruction_size, &symbols_table, &object_file,
                                      line_number);
            }
            /* Generate code for directive */
            else
            {
                is_directive = evaluate_directive(&elements,
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
    
    /* Check for errors */
    check_undefined_labels(&symbols_table);
    check_writing_at_const(&constants_table, &write_list, write_num);
    
    /* Printing */
    object_file_print(object_file);
    
    /* Writing */
    object_file_write(output, object_file);
    
    /* Finishing */
    object_file_destroy(&object_file);
    destroy_tables(&symbols_table, &instructions_table, &directives_table,
                   &constants_table);
    file_close(fp);
}

/**
 * Initialise all assembler tables.
 * @param symbols_table Allocated table for storing labels.
 * @param instructions_table Allocated table for storing instructions.
 * @param directives_table Allocated table for storing directives.
 * @param constants_table Allocated table for storing constants.
 */
void init_tables(hash_table_t *symbols_table, hash_table_t *instructions_table,
                 hash_table_t *directives_table, hash_table_t *constants_table)
{
    symbols_table_init(symbols_table);
    instructions_table_init(instructions_table);
    directives_table_init(directives_table);
    hash_create(constants_table, "Constants");
}

/**
 * Destroy all assembler tables.
 * @param symbols_table Allocated table for storing labels.
 * @param instructions_table Allocated table for storing instructions.
 * @param directives_table Allocated table for storing directives.
 * @param constants_table Allocated table for storing constants.
 */
void destroy_tables(hash_table_t *symbols_table, hash_table_t *instructions_table,
                    hash_table_t *directives_table, hash_table_t *constants_table)
{
    hash_destroy(symbols_table);
    hash_destroy(instructions_table);
    hash_destroy(directives_table);
    hash_destroy(constants_table);
}

/**
 * Evaluate a given label, adding to the symbols table when not in the table yet. If it's
 * already in the table, it needs to update all previous references to the label by its
 * value. It deals with array accessing by updating the label value added by an offset.
 * Also, it analyses errors such as invalidity of its name, redefinitions and jumps to
 * the data section.
 * @param label Given label for evaluation.
 * @param symbols_table Tables containing all symbols defined so far.
 * @param object_file_ptr Pointer to the object file.
 * @param line_number Number of the current line in the source file.
 */
void evaluate_label(element_t *elements, hash_table_t *symbols_table,
                    object_file_t *object_file_ptr, int line_number)
{
    symbol_t *symbol_ptr;
    int previous_position;
    int aux_position;
    int offset;
    char *label = elements->label;

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
        
            /*
             * The current label value is the last position in memory it was used.
             * This line must come before updating the value.
             */
            previous_position = symbol_ptr->value;
            
            /*
             * Label value it the place in memory it is pointing, which is the next
             * position. That's why it is not necessary to subtract 1 from the current
             * object file size.
             */
            symbol_ptr->value = object_file_ptr->size;
        
            /* Replace previous definitions */
            while (previous_position != -1)
            {
                /*
                 * Check whether jumping to data section for previous calls to the label,
                 * which happens when the instruction (available in the object file at the
                 * previous position-1) is a jump. Also, it can only happen when the data
                 * section is already defined, meaning that this label can be a constant
                 * with its value pointing to the data section. Now, if the data section
                 * is defined before the text section, the checking happen for values less
                 * than the text section address. Otherwise, checking for values greater
                 * than the data section are enough (meaning that the text section comes
                 * before the data one).
                 * The same reasoning applies to instructions that require a data memory
                 * address, such as arithmetic operations and store/input.
                 */
                switch (object_file_get(*object_file_ptr, previous_position-1))
                {
                    case JMP_OPCODE:
                    case JMPN_OPCODE:
                    case JMPP_OPCODE:
                    case JMPZ_OPCODE:
                        if (object_file_ptr->data_section_address >
                            object_file_ptr->text_section_address)
                        {
                            if ((object_file_ptr->data_section_address != -1) &&
                                (symbol_ptr->value >= object_file_ptr->data_section_address))
                                error_at_line(ERROR_SEMANTIC, symbol_ptr->line_number,
                                              "Jumping to data section");
                        }
                        else
                        {
                            if ((object_file_ptr->data_section_address != -1) &&
                                (symbol_ptr->value < object_file_ptr->text_section_address))
                                error_at_line(ERROR_SEMANTIC, symbol_ptr->line_number,
                                              "Jumping to data section");
                        }
                        break;
                        
                    case ADD_OPCODE:
                    case SUB_OPCODE:
                    case MULT_OPCODE:
                    case DIV_OPCODE:
                    case STORE_OPCODE:
                    case INPUT_OPCODE:
                        if (object_file_ptr->data_section_address >
                            object_file_ptr->text_section_address)
                        {
                            if ((object_file_ptr->text_section_address != -1) &&
                                (symbol_ptr->value < object_file_ptr->data_section_address))
                                error_at_line(ERROR_SEMANTIC, symbol_ptr->line_number,
                                              "Using text memory address as data");
                        }
                        else
                        {
                            if ((object_file_ptr->text_section_address != -1) &&
                                (symbol_ptr->value >= object_file_ptr->text_section_address))
                                error_at_line(ERROR_SEMANTIC, symbol_ptr->line_number,
                                              "Using text memory address as data");
                        }
                        break;
                }
                
                /* Hold the next position in memory to go*/
                aux_position = object_file_get(*object_file_ptr, previous_position);
                
                /* Get offset for array accessing */
                offset = object_file_get_offset(*object_file_ptr, previous_position);
                
                /* Replace by adding the label value to the offset */
                object_file_insert(object_file_ptr, previous_position,
                                   symbol_ptr->value + offset);
                
                /* Go to the next position in memory */
                previous_position = aux_position;
            }
        }
    }
}

/**
 * Check whether a instruction is valid and evaluate its opcode, writing it to the object
 * file.
 * It puts the target operand for instructions that write to the memory for later checking
 * if the code is writing to a constant memory address.
 * Errors can happen when an instruction is defined in the data section or when using a
 * division instruction with a constant 0.
 * @return The instruction size if it is a valid instruction, 0 otherwise.
 */
int evaluate_instruction(element_t *elements,
                         hash_table_t *instructions_table, section_t section,
                         int line_number, object_file_t *object_file, list_t *write_list,
                         int *write_num)
{
    instruction_t *instruction_ptr;
    char *instruction = elements->operation;
    char *operand1 = elements->operand1;
    char *operand2 = elements->operand2;
    write_t *write_ptr;

    /* Only enters when the instruction is found in the instructions table */
    if ((instruction_ptr = hash_search(instructions_table, instruction)))
    {
        if (section == SECTION_DATA)
            error_at_line(ERROR_SEMANTIC, line_number, "Using instruction \"%s\" in "
                          "the data section", instruction);
    
        /* Write opcode to the object file */
        object_file_add(object_file, instruction_ptr->opcode);
        
        /* For later checking whether writing in const memory */
        if ((strcmp(instruction, "STORE") == 0) || (strcmp(instruction, "INPUT") == 0))
        {
            write_ptr = malloc(sizeof(write_t));
            strcpy(write_ptr->label, operand1);
            write_ptr->line_number = line_number;
            list_append(write_list, write_ptr);
        }
        else if (strcmp(instruction, "COPY") == 0)
        {
            write_ptr = malloc(sizeof(write_t));
            strcpy(write_ptr->label, operand2);
            write_ptr->line_number = line_number;
            list_append(write_list, write_ptr);
        }
        /* Check division by zero */
        else if ((strcmp(instruction, "DIV") == 0) && (strcmp(operand1, "0") == 0))
        {
            error_at_line(ERROR_SEMANTIC, line_number, "Dividing by zero");
        }
    
        return instruction_ptr->size;
    }
    
    return 0;
}

/**
 * Process an operand, returning its label name LABEL and offset value N when in the
 * notation LABEL[N].
 * Also check for not valid operand label name.
 * @param output Allocated string for the processed label name.
 * @param label Input label.
 * @param line_number Current line for error printing purposes.
 * @return The offset, which is zero if no offset was defined.
 */
int process_operand(char *output, char *label, int line_number)
{
    int i;
    int label_size = strlen(label);
    char num_str[strlen(label)];
    int is_copying = 0;
    int num_str_i = 0;
    int offset;
    
    if (!is_valid_operand(label))
        error_at_line(ERROR_LEXICAL, line_number, "\"%s\" is not a valid label name",
                      label);
    
    strcpy(output, label);
    
    for (i = 0; i < label_size; ++i)
    {
        /* Beginning of the offset, the output must end here */
        if (label[i] == '[')
        {
            output[i] = '\0';
            is_copying = 1;
            continue;
        }
        
        /* Copy the offset value for a string */
        if (is_copying)
        {
            num_str[num_str_i] = label[i];
            ++num_str_i;
        }
        
        /* Finishing the offset string, convert to integer and return */
        if (label[i] == ']')
        {
            offset = strtol(num_str, NULL, 0);
            return offset;
        }
    }
    
    /* Only enters when no closure ']' was found */
    if (is_copying)
        error_at_line(ERROR_SYNTACTIC, line_number,
                      "Missing closing ']' at label %s", output);
    
    return 0; /* No offset was found */
}

/**
 * Evaluate an operand, adding to the symbols table if it was never defined or called.
 * Otherwise, it is already replaced by the label value and its offset.
 * Gives error when the previous instruction does not accept any argument or when the
 * instruction is a jump and the label value is pointing to the data section.
 * @param instruction Instruction string.
 * @param operand1 Operand string.
 * @param instruction_size Size of the instruction in words.
 * @param symbols_table Table that stores all labels.
 * @param object_file Output object file.
 * @param line_number Current line for error printing purposes.
 */
void evaluate_operand1(element_t *elements, int instruction_size,
                       hash_table_t *symbols_table, object_file_t *object_file,
                       int line_number)
{
    char processed_operand[100]; /* Operand without the offset when in LABEL[N] format */
    symbol_t *symbol_ptr; /* For searching the symbols table */
    int offset;
    char *instruction = elements->operation;
    char *operand1 = elements->operand1;
    
    if (instruction_size == 1)
        error_at_line(ERROR_SYNTACTIC, line_number, "Instruction \"%s\" does "
                      "not accept arguments", instruction);
    
    offset = process_operand(processed_operand, operand1, line_number);
    
    /* Not in the symbols table, add it and its offset */
    if (!(symbol_ptr = hash_search(symbols_table, processed_operand)))
    {
        symbols_table_add(symbols_table, processed_operand, object_file->size, line_number);
        object_file_add_with_offset(object_file, -1, offset);
    }
    else
    {
        /*
         * Already in the symbols table and defined, write the label value and its offset
         * to the object file.
         */
        if (symbol_ptr->defined)
        {
            /* Check jumping to data section when the label is already defined.
             * This will only happen to operands if the data section comes before the text
             * section.
             */
            if ((strcmp(instruction, "JMP") == 0) ||
                (strcmp(instruction, "JMPN") == 0) ||
                (strcmp(instruction, "JMPP") == 0) ||
                (strcmp(instruction, "JMPZ") == 0))
            {
                if ((object_file->text_section_address != -1) &&
                    (symbol_ptr->value < object_file->text_section_address))
                    error_at_line(ERROR_SEMANTIC, line_number, "Jumping to data section");
            }
            else if ((strcmp(instruction, "ADD") == 0) ||
                     (strcmp(instruction, "SUB") == 0) ||
                     (strcmp(instruction, "MULT") == 0) ||
                     (strcmp(instruction, "DIV") == 0) ||
                     (strcmp(instruction, "STORE") == 0) ||
                     (strcmp(instruction, "INPUT") == 0))
            {
                if ((object_file->text_section_address != -1) &&
                    (symbol_ptr->value > object_file->text_section_address))
                    error_at_line(ERROR_SEMANTIC, line_number, "Using text memory address "
                                  "as data");
            }
            object_file_add(object_file, symbol_ptr->value + offset);
        }
        /*
         * Already in the symbols table but not defined, point to the location of the
         * label last call (stored as the symbol value in the symbols table) and its
         * offset.
         */
        else
        {
            object_file_add_with_offset(object_file, symbol_ptr->value, offset);
            symbol_ptr->value = object_file->size - 1;
        }
    }
}

void evaluate_operand2(element_t *elements, int instruction_size,
                       hash_table_t *symbols_table, object_file_t *object_file,
                       int line_number)
{
    char processed_operand[100];
    symbol_t *symbol_ptr;
    int offset;
    char *instruction = elements->operation;
    char *operand2 = elements->operand2;
    
    if (instruction_size == 2)
        error_at_line(ERROR_SYNTACTIC, line_number, "Instruction \"%s\" only "
                      "accepts one argument", instruction);
    
    /* Label accessing array memory using the format LABEL[x] */
    offset = process_operand(processed_operand, operand2, line_number);
    
    if (!(symbol_ptr = hash_search(symbols_table, processed_operand)))
    {
        symbols_table_add(symbols_table, processed_operand, object_file->size, line_number);
        object_file_add_with_offset(object_file, -1, offset);
    }
    else
    {
        if (symbol_ptr->defined)
        {
            object_file_add(object_file, symbol_ptr->value);
        }
        else
        {
            object_file_add_with_offset(object_file, symbol_ptr->value, offset);
            symbol_ptr->value = object_file->size - 1;
        }
    }
}

/**
 * Evaluate a directive.
 * Errors occur when using directives in the text section (except for the SECTION
 * directive) or by invalid use of the directive (wrong number of arguments or section
 * name).
 */
int evaluate_directive(element_t *elements,
                       hash_table_t *directives_table, hash_table_t *constants_table,
                       section_t *section, int *is_data_section_defined,
                       int *is_text_section_defined, int line_number,
                       object_file_t *object_file)
{
    directive_t *directive_ptr;
    const_t *constant;
    int space_num;
    int i;
    char *directive = elements->operation;
    
    /* Generate code for directive */
    if ((directive_ptr = hash_search(directives_table, directive)))
    {
        if (strcmp(directive, "CONST") == 0)
        {
            /* Error checking */
            if ((*section) == SECTION_TEXT)
                error_at_line(ERROR_SEMANTIC, line_number, "Using directive \"%s\" "
                              "in the text section", directive);
            
            if (element_has_operand2(elements))
                error_at_line(ERROR_SYNTACTIC, line_number, "CONST directive accepts "
                              "only one argument");
                              
            if (!(element_has_operand1(elements)))
                error_at_line(ERROR_SYNTACTIC, line_number, "CONST directive requires "
                              "one argument");
        
            /* Add the constant to the object file and insert to the constant table */
            object_file_add(object_file, strtol(elements->operand1, NULL, 0));
            constant = malloc(sizeof(const_t));
            hash_insert(constants_table, elements->label, constant);
        }
        else if (strcmp(directive, "SPACE") == 0)
        {
            if ((*section) == SECTION_TEXT)
                error_at_line(ERROR_SEMANTIC, line_number, "Using directive \"%s\" "
                              "in the text section", directive);
            
            /* Reserve one space by default or the number defined with the parameter */
            if (element_has_operand1(elements))
                space_num = strtol(elements->operand1, NULL, 0);
            else
                space_num = 1;
            
            /* Initializing SPACE with zero value */
            for (i = 0; i < space_num; ++i)
                object_file_add(object_file, 0);
        }
        else if (strcmp(directive, "SECTION") == 0)
        {
            if (strcmp(elements->operand1, "DATA") == 0)
            {
                if ((*is_data_section_defined) && (*is_text_section_defined))
                    error_at_line(ERROR_SEMANTIC, line_number, "Section \"%s\" is "
                                  "already defined", elements->operand1);
                
                *is_data_section_defined = object_file->size;
                object_file->data_section_address = object_file->size;
                *section = SECTION_DATA;
            }
            else if (strcmp(elements->operand1, "TEXT") == 0)
            {
                if ((*is_data_section_defined) && (*is_text_section_defined))
                    error_at_line(ERROR_SEMANTIC, line_number, "Section \"%s\" is "
                                  "already defined", elements->operand1);
                
                *is_text_section_defined = object_file->size;
                object_file->text_section_address = object_file->size;
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

/**
 * Check whether any label was left undefined in the symbols table.
 * @param symbols_table Allocated table containing all labels.
 */
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

/**
 * Check all labels in the write list and search it in the constant table. If any is found,
 * it means an instruction is attempting to write to a constant memory address and an error
 * should be raised.
 * @param constants_table Allocated table storing all program constants.
 * @param write_list List of labels used as targets by writing instructions.
 * @param write_num Number of elements in the write_list.
 */
void check_writing_at_const(hash_table_t *constants_table, list_t *write_list,
                            int write_num)
{
    const_t *constant;
    char *label;
    list_node_t *current_node = write_list->head;
    
    while (current_node != NULL)
    {
        label = ((write_t*)current_node->data)->label;
        
        if ((constant = hash_search(constants_table, label)))
        {
            error_at_line(ERROR_SEMANTIC, ((write_t*)current_node->data)->line_number, "Cannot write to the "
                          "constant label %s", label);
        }
        
        current_node = current_node->next;
    }
}