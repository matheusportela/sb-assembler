/**
 * @file   preprocessor.c
 * @author Matheus Vieira Portela
 * @author Lucas de Levy Oliveira
 * @date   17/04/2014
 *
 * @brief  Implements pre-processing
 *
 * The preprocessing is implemented with a two-pass algorithm. The first pass only detects
 * equate directives and build an equate table with its value. The second pass removes
 * comments, replaces the equates labels and evaluate the if conditions.
 * Also, it makes everything case-insensitive by forcing the whole source code to
 * uppercase.
 */

#include "preprocessor.h"

/**
 * Preprocess a source code, generating a preprocessed output file.
 * @param filename Input source code.
 * @param output Output preprocessed code.
 */
void preprocess(char *filename, char *output)
{
    hash_table_t equate_table;
    
    printf("===== Pre-processing =====\n");
    
    equate_table_init(&equate_table);
    preprocessor_first_pass(filename, &equate_table);
    preprocessor_second_pass(filename, output, &equate_table);
}

/**
 * First pass, detects equate directives and force the code to uppercase.
 * @param filename Input source file.
 * @param equate_table Allocated table to store the equate directives.
 */
void preprocessor_first_pass(char *filename, hash_table_t *equate_table)
{
    FILE *fp;
    char line_buffer[FILE_LINE_LENGTH];
    element_t elements;
    
    /* Open files */
    fp = file_open(filename, "r");
    
    /* Detect EQU directives */
    while(file_read_line(fp, line_buffer) != FILE_FINISHED)
    {
        remove_comments(line_buffer);
        
        /* Make it case insensitive by forcing everything to uppercase */
        to_uppercase(line_buffer);
        
        if (detect_directive(&elements, line_buffer) == DIRECTIVE_EQU_NUMBER)
            equate_table_add(equate_table, elements.label, elements.operand1);
        
        element_clear(&elements); /* So as one line does not interfere to the other */
    }
    
    /* Close files */
    file_close(fp);
}

/**
 * Second pass, force the code to uppercase, remove comments, replace equate directives
 * and evaluate if cases. It must evaluate the equates before the if conditions.
 * For false IF cases, the next line must be ignored.
 * @param filename Input source file.
 * @param output Output preprocessed file.
 * @param equate_table Allocated table to store the equate directives.
 */
void preprocessor_second_pass(char *filename, char *output, hash_table_t *equate_table)
{
    FILE *fp;
    FILE *fout;
    char line_buffer[FILE_LINE_LENGTH];
    element_t elements;
    equate_t *equate;
    int is_equate = 0;
    int is_if = 0;
    int is_if_false = 0;
    
    /* Open files */
    fp = file_open(filename, "r");
    fout = file_open(output, "w");
    
    while(file_read_line(fp, line_buffer) != FILE_FINISHED)
    {
        remove_comments(line_buffer);
        
        /* Make it case insensitive by forcing everything to uppercase */
        to_uppercase(line_buffer);
        
        /* Replace EQU directives */
        scan_line_elements(&elements, line_buffer);
        equate = hash_search(equate_table, elements.operand1);
        
        if (equate)
            replace(line_buffer, elements.operand1, equate->value);
        
        if (strcmp(elements.operation, "EQU") == 0)
            is_equate = 1;
            
        element_clear(&elements); /* So as one line does not interfere to the other */
        
        /* Ignore line if IF directive was false */
        if (is_if_false)
        {
            is_if_false = 0;
            continue;
        }
        
        /* Evaluate IF directives */
        scan_line_elements(&elements, line_buffer);
        
        if (strcmp(elements.operation, "IF") == 0)
        {
            is_if = 1;
            
            /* Set flag if IF directive is evaluated as false */
            if (!strtol(elements.operand1, NULL, 0))
                is_if_false = 1;
            
        }
        
        element_clear(&elements); /* So as one line does not interfere to the other */
        
        /* Write line */
        if (is_if)
        {
            is_if = 0;
            continue;
        }
        
        if (is_equate)
        {
            is_equate = 0;
            continue;
        }
        
        printf("%s", line_buffer);
        fprintf(fout, "%s", line_buffer);
    }
    
    /* Close files */
    file_close(fp);
    file_close(fout);
}

/**
 * Remove comments by placing "\n\0" to the position where a ";" is found.
 * @param line Line to have its comments removed.
 */
void remove_comments(char *line)
{
    int i;
    
    for(i = 0; line[i] != '\0'; ++i)
    {
        switch (line[i])
        {
            case ';':
            case -1: /* This one is weird and appears on the last word of the file */
                line[i] = '\n';
                line[i+1] = '\0';
                return;
        }
    }
}

/**
 * Detect whether a line has either an if or an equate directive, also storing the parsed
 * line to an element struct.
 * @param elements Store the parsed line.
 * @param line Line to be evaluated.
 */
int detect_directive(element_t *elements, char *line)
{
    scan_line_elements(elements, line);
    
    if (strcmp(elements->operation, "IF") == 0)
        return DIRECTIVE_IF_NUMBER;
    else if (strcmp(elements->operation, "EQU") == 0)
        return DIRECTIVE_EQU_NUMBER;
    
    return NO_DIRECTIVE;
}

/**
 * Replace a substring in a string by another one. Following the implementation found at:
 * http://creativeandcritical.net/str-replace-c/
 * @param str String that will have its elements replaced
 * @param old String to be replaced
 * @param new Replacing string
 */
void replace(char *str, char *old, char *new)
{
    char *ret;
    char *r;
    const char *p;
    const char *q;
    size_t oldlen = strlen(old);
    size_t newlen = strlen(new);
    size_t retlen;
    size_t count;

    if (oldlen != newlen)
    {
        for (count = 0, p = str; (q = strstr(p, old)) != NULL; p = q + oldlen)
            count++;
            
        /* This is undefined if p - str > PTRDIFF_MAX */
        retlen = p - str + strlen(p) + count*(newlen - oldlen);
    }
    else
    {
        retlen = strlen(str);
    }

    /* Could not allocate memory */
    ret = malloc(retlen + 1);

    for (r = ret, p = str; (q = strstr(p, old)) != NULL; p = q + oldlen)
    {
        /* This is undefined if q - p > PTRDIFF_MAX */
        ptrdiff_t l = q - p;
        memcpy(r, p, l);
        r += l;
        memcpy(r, new, newlen);
        r += newlen;
    }
    
    strcpy(r, p);
    strcpy(str, ret);
}

/**
 * Convert a string to uppercase.
 * @param token String to be converted
 */
void to_uppercase(char *token)
{
    int i;
    for (i = 0; token[i] != '\0'; ++i)
        token[i] = toupper(token[i]);
}