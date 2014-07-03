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
#include "asm_opcodes_table.h"
#include "translation_table.h"

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
    FILE *fout;
    object_file_t object;
    hash_table_t asm_opcodes_table;
    hash_table_t translation_table;
    asm_opcode_t *asm_opcode_ptr;
    translation_t *translation_ptr;
    int i, j;
    char asm_opcode_str[10];
    char operation[7];
    char trans_operation[100];
    int num_args;
    int args[2];
    int *labels_list;
    char label1[10];
    char label2[10];
    int *instruction_to_line_table;
    int line_number = 0;
    int value;
    char **output_lines;
    char outline[1024];
    unsigned char opcodes[4096];
    int opcodes_size = 0;

    printf("===== Translating =====\n");
    
    fout = file_open(output, "w");
    asm_opcodes_table_init(&asm_opcodes_table);
    translation_table_init(&translation_table);
    object_file_read(input, &object);
    
    labels_list = malloc(sizeof(labels_list)*object.size);
    memset(labels_list, -1, sizeof(labels_list)*object.size);
    
    instruction_to_line_table = malloc(sizeof(instruction_to_line_table)*object.size);
    
    output_lines = malloc(sizeof(*output_lines)*object.size);
    
    printf("Text section: %d\nData section: %d\n\n", object.text_section_address, object.data_section_address);
    
    /* Analysing the instructions */
    fprintf(fout, "extern LerInteiro\n"
                  "extern EscreverInteiro\n"
                  "\n"
                  "section .text\n"
                  "global _start\n"
                  "_start:\n");
    for (i = object.text_section_address; i < object.size; ++i)
    {
        /* Find the instruction from the ASM opcode value */
        sprintf(asm_opcode_str, "%d", object_file_get(object, i));
        asm_opcode_ptr = hash_search(&asm_opcodes_table, asm_opcode_str);
        if (!asm_opcode_ptr) /* There is no such instruction, ignore everything afterwards */
            error(ERROR_TRANSLATION, "Unknown instruction %d", asm_opcode_ptr);
        strcpy(operation, asm_opcode_ptr->instruction);
        num_args = asm_opcode_ptr->size - 1;
        
        instruction_to_line_table[i] = line_number;
        
        /* Get the ASM opcode parameters */
        for (j = 0; j < num_args; ++j)
            args[j] = object_file_get(object, i+j+1);
        
        /* Rebuilt instruction */
        printf("%s ", operation);
        for (j = 0; j < num_args; ++j)
        {
            instruction_to_line_table[i+j+1] = line_number;
        
            if (object.data_section_address < object.text_section_address)
            {
                if (args[j] < object.text_section_address)
                {
                    /* Must be in data section */
                    if (j == 0)
                        sprintf(label1, "DATA%d", args[j] - object.data_section_address);
                    else
                        sprintf(label2, "DATA%d", args[j] - object.data_section_address);
                }
                else
                {
                    /* Must be in text section */
                    if (j == 0)
                        sprintf(label1, "TEXT%d", args[j] - object.text_section_address);
                    else
                        sprintf(label2, "TEXT%d", args[j] - object.text_section_address);
                    labels_list[args[j]] = args[j] - object.text_section_address;
                }
            }
            else
            {
                if (args[j] < object.data_section_address)
                {
                    /* Must be in text section */
                    if (j == 0)
                        sprintf(label1, "TEXT%d", args[j] - object.text_section_address);
                    else
                        sprintf(label2, "TEXT%d", args[j] - object.text_section_address);
                    labels_list[args[j]] = args[j] - object.text_section_address;
                }
                else
                {
                    /* Must be in data section */
                    if (j == 0)
                        sprintf(label1, "DATA%d", args[j] - object.data_section_address);
                    else
                        sprintf(label2, "DATA%d", args[j] - object.data_section_address);
                }
            }
            
            printf("%d ", args[j]);
        }
        printf("-> ");
        
        
        /* Translating */
        translation_ptr = hash_search(&translation_table, operation);
        strcpy(trans_operation, translation_ptr->instruction);
        
        if (strcmp(operation, "ADD") == 0)
        {
            sprintf(trans_operation, "add word ax, [%s]", label1);
            
            /* add word ax, [label] */
            opcodes[opcodes_size] = 0x66;
            opcodes[opcodes_size + 1] = 0x03;
            opcodes[opcodes_size + 2] = 0x05;
            opcodes[opcodes_size + 3] = 0x00; /* don't care */
            opcodes[opcodes_size + 4] = 0x00; /* don't care */
            opcodes[opcodes_size + 5] = 0x00; /* don't care */
            opcodes[opcodes_size + 6] = 0x00; /* don't care */
            opcodes_size += 7;
        }
        else if (strcmp(operation, "SUB") == 0)
        {
            sprintf(trans_operation, "sub word ax, [%s]", label1);
            
            /* sub word ax, [label] */
            opcodes[opcodes_size] = 0x66;
            opcodes[opcodes_size + 1] = 0x2B;
            opcodes[opcodes_size + 2] = 0x05;
            opcodes[opcodes_size + 3] = 0x00; /* don't care */
            opcodes[opcodes_size + 4] = 0x00; /* don't care */
            opcodes[opcodes_size + 5] = 0x00; /* don't care */
            opcodes[opcodes_size + 6] = 0x00; /* don't care */
            opcodes_size += 7;
        }
        else if (strcmp(operation, "MULT") == 0)
        {
            sprintf(trans_operation, "mov word bx, [%s]\n"
                                     "imul word bx", label1);
            
            /* mov word bx, [label] */
            opcodes[opcodes_size] = 0x66;
            opcodes[opcodes_size + 1] = 0x8B;
            opcodes[opcodes_size + 2] = 0x1D;
            opcodes[opcodes_size + 3] = 0x00; /* don't care */
            opcodes[opcodes_size + 4] = 0x00; /* don't care */
            opcodes[opcodes_size + 5] = 0x00; /* don't care */
            opcodes[opcodes_size + 6] = 0x00; /* don't care */
            opcodes_size += 7;
            
            /* imul word bx */
            opcodes[opcodes_size] = 0x66;
            opcodes[opcodes_size + 1] = 0xF7;
            opcodes[opcodes_size + 2] = 0xEB;
            opcodes_size += 3;
                                     
        }
        else if (strcmp(operation, "DIV") == 0)
        {
            sprintf(trans_operation, "cwd\n"
                                     "mov word bx, [%s]\n"
                                     "idiv word bx", label1);
                                     
            /* cwd */
            opcodes[opcodes_size] = 0x66;
            opcodes[opcodes_size + 1] = 0x99;
            opcodes_size += 2;
            
            /* mov word bx, [label] */
            opcodes[opcodes_size] = 0x66;
            opcodes[opcodes_size + 1] = 0x8B;
            opcodes[opcodes_size + 2] = 0x1D;
            opcodes[opcodes_size + 3] = 0x00; /* don't care */
            opcodes[opcodes_size + 4] = 0x00; /* don't care */
            opcodes[opcodes_size + 5] = 0x00; /* don't care */
            opcodes[opcodes_size + 6] = 0x00; /* don't care */
            opcodes_size += 7;
            
            /* idiv word bx */
            opcodes[opcodes_size] = 0x66;
            opcodes[opcodes_size + 1] = 0xF7;
            opcodes[opcodes_size + 2] = 0xFB;
            opcodes_size += 3;
        }
        else if (strcmp(operation, "JMP") == 0)
        {
            sprintf(trans_operation, "jmp %s", label1);
            
            /* jmp label */
            opcodes[opcodes_size] = 0xEB;
            opcodes[opcodes_size + 1] = 0x00; /* don't care */ /* short jump */
            opcodes_size += 2;
        }
        else if (strcmp(operation, "JMPN") == 0)
        {
            sprintf(trans_operation, "cmp ax, 0\n"
                                     "jl %s", label1);
            
            /* cmp ax, 0 */
            opcodes[opcodes_size] = 0x66;
            opcodes[opcodes_size + 1] = 0x83;
            opcodes[opcodes_size + 2] = 0xF8;
            opcodes[opcodes_size + 3] = 0x00;
            opcodes_size += 4;
            
            /* jl label */
            opcodes[opcodes_size] = 0x7C;
            opcodes[opcodes_size + 1] = 0x00; /* don't care */ /* short jump */
            opcodes_size += 2;
        }
        else if (strcmp(operation, "JMPP") == 0)
        {
            sprintf(trans_operation, "cmp ax, 0\n"
                                     "jg %s", label1);
            
            /* cmp ax, 0 */
            opcodes[opcodes_size] = 0x66;
            opcodes[opcodes_size + 1] = 0x83;
            opcodes[opcodes_size + 2] = 0xF8;
            opcodes[opcodes_size + 3] = 0x00;
            opcodes_size += 4;
            
            /* jg label */
            opcodes[opcodes_size] = 0x7F;
            opcodes[opcodes_size + 1] = 0x00; /* don't care */ /* short jump */
            opcodes_size += 2;
        }
        else if (strcmp(operation, "JMPZ") == 0)
        {
            sprintf(trans_operation, "cmp ax, 0\n"
                                     "je %s", label1);
            
            /* cmp ax, 0 */
            opcodes[opcodes_size] = 0x66;
            opcodes[opcodes_size + 1] = 0x83;
            opcodes[opcodes_size + 2] = 0xF8;
            opcodes[opcodes_size + 3] = 0x00;
            opcodes_size += 4;
            
            /* je label */
            opcodes[opcodes_size] = 0x74;
            opcodes[opcodes_size + 1] = 0x00; /* don't care */ /* short jump */
            opcodes_size += 2;
        }
        else if (strcmp(operation, "COPY") == 0)
        {
            sprintf(trans_operation, "mov word bx, [%s]\n"
                                     "mov word [%s], bx", label1, label2);
            
            /* mov word bx, [label] */
            opcodes[opcodes_size] = 0x66;
            opcodes[opcodes_size + 1] = 0x8B;
            opcodes[opcodes_size + 2] = 0x1D;
            opcodes[opcodes_size + 3] = 0x00; /* don't care */
            opcodes[opcodes_size + 4] = 0x00; /* don't care */
            opcodes[opcodes_size + 5] = 0x00; /* don't care */
            opcodes[opcodes_size + 6] = 0x00; /* don't care */
            opcodes_size += 7;
            
            /* mov word [label], bx */
            opcodes[opcodes_size] = 0x66;
            opcodes[opcodes_size + 1] = 0x89;
            opcodes[opcodes_size + 2] = 0x1D;
            opcodes[opcodes_size + 3] = 0x00; /* don't care */
            opcodes[opcodes_size + 4] = 0x00; /* don't care */
            opcodes[opcodes_size + 5] = 0x00; /* don't care */
            opcodes[opcodes_size + 6] = 0x00; /* don't care */
            opcodes_size += 7;
        }
        else if (strcmp(operation, "LOAD") == 0)
        {
            sprintf(trans_operation, "mov word ax, [%s]", label1);
            
            /* mov word ax, [label] */
            opcodes[opcodes_size] = 0x66;
            opcodes[opcodes_size + 1] = 0xA1;
            opcodes[opcodes_size + 2] = 0x00; /* don't care */
            opcodes[opcodes_size + 3] = 0x00; /* don't care */
            opcodes[opcodes_size + 4] = 0x00; /* don't care */
            opcodes[opcodes_size + 5] = 0x00; /* don't care */
            opcodes_size += 6;
        }
        else if (strcmp(operation, "STORE") == 0)
        {
            sprintf(trans_operation, "mov word [%s], ax", label1);
            
            /* mov word [label], ax */
            opcodes[opcodes_size] = 0x66;
            opcodes[opcodes_size + 1] = 0xA3;
            opcodes[opcodes_size + 2] = 0x00; /* don't care */
            opcodes[opcodes_size + 3] = 0x00; /* don't care */
            opcodes[opcodes_size + 4] = 0x00; /* don't care */
            opcodes[opcodes_size + 5] = 0x00; /* don't care */
            opcodes_size += 6;
        }
        else if (strcmp(operation, "INPUT") == 0)
        {
            sprintf(trans_operation, "call LerInteiro\n"
                                     "mov word [%s], bx", label1);
            
            /* call LerInteiro */
            opcodes[opcodes_size] = 0xE8;
            opcodes[opcodes_size + 1] = 0x00;
            opcodes[opcodes_size + 2] = 0x00; /* LerInteiro addr */
            opcodes[opcodes_size + 3] = 0x00;
            opcodes[opcodes_size + 4] = 0x00;
            opcodes_size += 5;
            
            /* mov word [label], bx */
            opcodes[opcodes_size] = 0x66;
            opcodes[opcodes_size + 1] = 0x89;
            opcodes[opcodes_size + 2] = 0x1D;
            opcodes[opcodes_size + 3] = 0x00; /* don't care */
            opcodes[opcodes_size + 4] = 0x00; /* don't care */
            opcodes[opcodes_size + 5] = 0x00; /* don't care */
            opcodes[opcodes_size + 6] = 0x00; /* don't care */
            opcodes_size += 7;
        }
        else if (strcmp(operation, "OUTPUT") == 0)
        {
            sprintf(trans_operation, "mov word bx, [%s]\n"
                                     "call EscreverInteiro", label1);
            
            /* mov word bx, [label] */
            opcodes[opcodes_size] = 0x66;
            opcodes[opcodes_size + 1] = 0x8B;
            opcodes[opcodes_size + 2] = 0x1D;
            opcodes[opcodes_size + 3] = 0x00; /* don't care */
            opcodes[opcodes_size + 4] = 0x00; /* don't care */
            opcodes[opcodes_size + 5] = 0x00; /* don't care */
            opcodes[opcodes_size + 6] = 0x00; /* don't care */
            opcodes_size += 7;
            
            /* call EscreverInteiro */
            opcodes[opcodes_size] = 0xE8;
            opcodes[opcodes_size + 1] = 0x00;
            opcodes[opcodes_size + 2] = 0x00; /* EscreverInteiro addr */
            opcodes[opcodes_size + 3] = 0x00;
            opcodes[opcodes_size + 4] = 0x00;
            opcodes_size += 5;
        }
        else if (strcmp(operation, "STOP") == 0)
        {
            sprintf(trans_operation, "mov eax, 1\n"
			                         "mov ebx, 0\n"
			                         "int 80h");
            
            /* mov eax, 1 */
            opcodes[opcodes_size] = 0xB8;
            opcodes[opcodes_size + 1] = 0x01;
            opcodes[opcodes_size + 2] = 0x00;
            opcodes[opcodes_size + 3] = 0x00;
            opcodes[opcodes_size + 4] = 0x00;
            
            /* mov ebx, 0 */
            opcodes[opcodes_size + 5] = 0xBB;
            opcodes[opcodes_size + 6] = 0x00;
            opcodes[opcodes_size + 7] = 0x00;
            opcodes[opcodes_size + 8] = 0x00;
            opcodes[opcodes_size + 9] = 0x00;
            
            /* int 80h */
            opcodes[opcodes_size + 10] = 0xCD;
            opcodes[opcodes_size + 11] = 0x80;
            opcodes_size += 12;
        }
        
        printf("%s\n", trans_operation);
        sprintf(outline, "%s\n", trans_operation);
        
        output_lines[line_number] = malloc(sizeof(char)*256);
        strcpy(output_lines[line_number], outline);
        
        /* Skip for the number of parameters */
        i += asm_opcode_ptr->size - 1;
        line_number += 1;
        
        /* After an STOP instruction, everything will be data (or the program is over) */
        if (strcmp(asm_opcode_ptr->instruction, "STOP") == 0)
            break;
    }
    
    /* Giving labels to text section */
    printf("\nLabels list\n");
    for (i = 0; i < object.size; ++i)
    {
        if (labels_list[i] != -1)
        {
            /*printf("%d: %d -> %d\n", i, labels_list[i], instruction_to_line_table[labels_list[i]]);*/
            sprintf(outline, "TEXT%d: ", labels_list[i]);
            strcat(outline, output_lines[instruction_to_line_table[labels_list[i]]]);
            /*printf("%d, %s", instruction_to_line_table[labels_list[i]], outline);*/
            strcpy(output_lines[instruction_to_line_table[labels_list[i]]], outline);
        }
    }
    
    printf("\nOutput lines\n");
    for (i = 0; i < line_number; ++i)
    {
        printf("%s", output_lines[i]);
        fprintf(fout, "%s", output_lines[i]);
        free(output_lines[i]);
    }
    free(output_lines);
    
    fprintf(fout, "\nsection .data\n");
    for (i = object.data_section_address; i < object.size; ++i)
    {
        /* Data section finished and text section is beginning */
        if ((object.data_section_address < object.text_section_address)
           && (i >= object.text_section_address))
           break;
    
        value = object_file_get(object, i);
        sprintf(trans_operation, "DATA%d dw %d", (i - object.data_section_address), value);
        printf("%s\n", trans_operation);
        
        fprintf(fout, "%s\n", trans_operation);
    }
    
    printf("Opcodes:\n");
    
    j = 0;
    for (i = 0; i < opcodes_size; ++i)
    {
        printf("%2.2x", opcodes[i]);
        
        if (j == 15)
        {
            printf("\n");
            j = 0;
            continue;
        }
        
        if ((j+1) % 4 == 0)
        {
            printf(" ");
        }
        
        ++j;
    }
    printf("\n");
    
    hash_destroy(&asm_opcodes_table);
    hash_destroy(&translation_table);
    free(labels_list);
    fclose(fout);
}
