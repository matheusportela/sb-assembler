#include "assembler.h"

typedef enum
{
    SECTION_UNKNOWN,
    SECTION_DATA,
    SECTION_TEXT,
} section_t;

/**
 * Main function
 * Init all required tables, run passes, and free tables.
 */

void parse_arguments(int argc, char **argv, char **infile, char **prefile, char **outfile)
{
    if (argc != 4)
        error(ERROR_COMMAND_LINE, "Wrong number of arguments\n"
              "Usage: assembler <input> <preprocessing> <output>");
    
    *infile = argv[1];
    *prefile = argv[2];
    *outfile = argv[3];
    
    printf("===== Parsing arguments =====\n");
    printf("Input file: %s\n", *infile);
    printf("Pre-processing file: %s\n", *prefile);
    printf("Output file: %s\n", *outfile);
    printf("\n");
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

void assemble(char *input, char *output)
{
    const int DEBUG = 1;
    hash_table_t symbols_table;
    hash_table_t instructions_table;
    hash_table_t directives_table;
    object_file_t object_file;
    FILE *fp;
    element_t elements;
    char line_buffer[FILE_LINE_LENGTH];
    symbol_t *symbol_ptr;
    instruction_t *instruction_ptr;
    directive_t *directive_ptr;
    int previous_position;
    int aux_position;
    section_t section = SECTION_UNKNOWN;
    int line_number = 0;
    int is_instruction = 0;
    int is_directive = 0;
    int is_data_section_defined = 0;
    int is_text_section_defined = 0;
    char *label_ptr;
    hash_list_node_t *hash_list_node_ptr;
    
    /* Initializing */
    init_tables(&symbols_table, &instructions_table, &directives_table);
    object_file_init(&object_file);
    fp = file_open(input, "r");
    
    /* Assembling */
    printf("===== Assembling =====\n");
    
    element_init(&elements); /* Avoid garbage values at the label field */
    
    printf("=== Parsing ===\n");
    while(file_read_line(fp, line_buffer) != FILE_FINISHED)
    {
        is_instruction = 0;
        is_directive = 0;
        line_number += 1;
        
        /* Scanning */
        scan_line_elements(&elements, line_buffer);
        if (DEBUG)
            printf("%15.15s | %15.15s | %15.15s | %15.15s\n", elements.label, elements.operation,
                   elements.operand1, elements.operand2);
        
        /* Label analysis */
        if (element_has_label(&elements))
        {
            if (!is_valid_label(elements.label))
                error_at_line(ERROR_LEXICAL, line_number, "\"%s\" is not a valid label name",
                              elements.label);
            
            /* Label not in the table yet */
            if (!(symbol_ptr = hash_search(&symbols_table, elements.label)))
            {
                symbols_table_add(&symbols_table, elements.label, object_file.size, line_number);
                symbol_ptr = hash_search(&symbols_table, elements.label);
                symbol_ptr->defined = 1;
            }
            else
            {
                if (symbol_ptr->defined)
                {
                    error_at_line(ERROR_SEMANTIC, line_number, "Redefined label \"%s\"",
                                  elements.label);
                }
                else
                {
                    /* Label in the table but not defined */
                    symbol_ptr->defined = 1;
                
                    previous_position = symbol_ptr->value;
                    symbol_ptr->value = object_file.size;
                
                    /* Replace previous definitions */
                    while (previous_position != -1)
                    {
                        aux_position = object_file_get(object_file, previous_position);
                        object_file_insert(&object_file, previous_position, symbol_ptr->value);
                        previous_position = aux_position;
                    }
                }
            }
        }
        
        /* Generate code for instruction */
        if ((instruction_ptr = hash_search(&instructions_table, elements.operation)))
        {
            if (section == SECTION_DATA)
                error_at_line(ERROR_SEMANTIC, line_number, "Using instruction \"%s\" in "
                              "the data section", elements.operation);
        
            is_instruction = 1;
            object_file_add(&object_file, instruction_ptr->opcode);
        
            /* Generate code for labels in operands */
            if (element_has_operand1(&elements))
            {
                if (instruction_ptr->size == 1)
                    error_at_line(ERROR_SYNTACTIC, line_number, "Instruction \"%s\" does "
                                  "not accept arguments", elements.operation);
            
                if (!(symbol_ptr = hash_search(&symbols_table, elements.operand1)))
                {
                    symbols_table_add(&symbols_table, elements.operand1, object_file.size, line_number);
                    object_file_add(&object_file, -1);
                }
                else
                {
                    if (symbol_ptr->defined)
                    {
                        object_file_add(&object_file, symbol_ptr->value);
                    }
                    else
                    {
                        object_file_add(&object_file, symbol_ptr->value);
                        symbol_ptr->value = object_file.size - 1;
                    }
                }
            }
        
            if (element_has_operand2(&elements))
            {
                if (instruction_ptr->size == 2)
                    error_at_line(ERROR_SYNTACTIC, line_number, "Instruction \"%s\" only "
                                  "accepts one argument", elements.operation);
            
                if (!(symbol_ptr = hash_search(&symbols_table, elements.operand2)))
                {
                    symbols_table_add(&symbols_table, elements.operand2, object_file.size, line_number);
                    object_file_add(&object_file, -1);
                }
                else
                {
                    if (symbol_ptr->defined)
                    {
                        object_file_add(&object_file, symbol_ptr->value);
                    }
                    else
                    {
                        object_file_add(&object_file, symbol_ptr->value);
                        symbol_ptr->value = object_file.size - 1;
                    }
                }
            }
        }
        
        /* Generate code for directive */
        if ((directive_ptr = hash_search(&directives_table, elements.operation)))
        {
            is_directive = 1;
            if (strcmp(elements.operation, "CONST") == 0)
            {
                if (section == SECTION_TEXT)
                    error_at_line(ERROR_SEMANTIC, line_number, "Using directive \"%s\" "
                                  "int the data section", elements.operation);
            
                if (element_has_operand1(&elements))
                    object_file_add(&object_file, strtol(elements.operand1, NULL, 0));
                else
                    error_at_line(ERROR_SYNTACTIC, line_number, "CONST directive requires "
                                  "one argument");
                
                if (element_has_operand2(&elements))
                    error_at_line(ERROR_SYNTACTIC, line_number, "CONST directive accepts "
                                  "only one argument");
            }
            else if (strcmp(elements.operation, "SPACE") == 0)
            {
                if (section == SECTION_TEXT)
                    error_at_line(ERROR_SEMANTIC, line_number, "Using directive \"%s\" "
                                  "int the data section", elements.operation);
            
                object_file_add(&object_file, 0); /* Initializing SPACE with zero value */
            }
            else if (strcmp(elements.operation, "SECTION") == 0)
            {
                if (strcmp(elements.operand1, "DATA") == 0)
                {
                    if (is_data_section_defined && is_text_section_defined)
                        error_at_line(ERROR_SEMANTIC, line_number, "Section \"%s\" is "
                                      "already defined", elements.operand1);
                    
                    is_data_section_defined = 1;
                    section = SECTION_DATA;
                }
                else if (strcmp(elements.operand1, "TEXT") == 0)
                {
                    if (is_data_section_defined && is_text_section_defined)
                        error_at_line(ERROR_SEMANTIC, line_number, "Section \"%s\" is "
                                      "already defined", elements.operand1);
                    
                    is_text_section_defined = 1;
                    section = SECTION_TEXT;
                }
                else
                {
                    error_at_line(ERROR_SYNTACTIC, line_number, "Unknown section directive "
                                  "\"%s\"", elements.operand1);
                }
                
                if (element_has_operand2(&elements))
                    error_at_line(ERROR_SYNTACTIC, line_number, "SECTION directive accepts "
                                  "only one argument");
            }
        }
        
        if ((element_has_operation(&elements)) && (!is_instruction) && (!is_directive))
            error_at_line(ERROR_LEXICAL, line_number, "\"%s\" is not a valid instruction "
                          "or directive", elements.operation);
        
        element_clear(&elements); /* So as one line does not interfere to the other */
    }
    printf("\n");
    
    /* Check for undefined labels */
    if ((hash_list_node_ptr = symbols_table_has_undefined(&symbols_table)))
    {
        label_ptr = hash_list_node_ptr->key;
        line_number = ((symbol_t*)hash_list_node_ptr->data)->line_number;
        error_at_line(ERROR_SEMANTIC, line_number, "Undefined label \"%s\"", label_ptr);
    }
    
    /* Printing */
    object_file_print(object_file);
    
    /* Writing */
    object_file_write(output, object_file);
    
    /* Finishing */
    object_file_destroy(&object_file);
    destroy_tables(&symbols_table, &instructions_table, &directives_table);
    file_close(fp);
}

int main(int argc, char **argv)
{
    char *infile, *prefile, *outfile;
    
    parse_arguments(argc, argv, &infile, &prefile, &outfile);
    printf("===== Pre-processing =====\n");
    preprocess(infile, prefile);
    assemble(prefile, outfile);
    
    object_file_read(outfile);
    return 0;
}