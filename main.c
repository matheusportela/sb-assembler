#include "assembler.h"

/**
 * Main function
 * Init all required tables, run passes, and free tables.
 */

void parse_arguments(int argc, char **argv, char **infile, char **prefile, char **outfile)
{
    if (argc != 4)
    {
        fprintf(stderr, "ERROR: Wrong number of arguments\n");
        fprintf(stderr, "Usage: assembler <input> <preprocessing> <output>\n");
        exit(-1);
    }
    
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
    int previous_position, aux_position;
    
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
        /* Scanning */
        scan_line_elements(&elements, line_buffer);
        if (DEBUG)
            printf("%15.15s | %15.15s | %15.15s | %15.15s\n", elements.label, elements.operation,
                   elements.operand1, elements.operand2);
        
        /* Label analysis */
        if (element_has_label(&elements))
        {
            /* Label not in the table yet */
            if (!(symbol_ptr = hash_search(&symbols_table, elements.label)))
            {
                symbols_table_add(&symbols_table, elements.label, object_file.size);
                symbol_ptr = hash_search(&symbols_table, elements.label);
                symbol_ptr->defined = 1;
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
                
                /* TODO: If label already in the table and defined -> ERROR */
            }
        }
        
        /* Generate code for instruction */
        if ((instruction_ptr = hash_search(&instructions_table, elements.operation)))
        {
            object_file_add(&object_file, instruction_ptr->opcode);
        
            /* Generate code for labels in operands */
            if (element_has_operand1(&elements))
            {
                if (!(symbol_ptr = hash_search(&symbols_table, elements.operand1)))
                {
                    symbols_table_add(&symbols_table, elements.operand1, object_file.size);
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
                if (!(symbol_ptr = hash_search(&symbols_table, elements.operand2)))
                {
                    symbols_table_add(&symbols_table, elements.operand2, object_file.size);
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
            if (element_has_operand1(&elements))
            {
                object_file_add(&object_file, strtol(elements.operand1, NULL, 0));
            }
            else
            {
                object_file_add(&object_file, 0); /* Initializing SPACE with zero value */
            }
        }
        
        element_clear(&elements); /* So as one line does not interfere to the other */
    }
    printf("\n");
    
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