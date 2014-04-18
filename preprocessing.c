/**
 * @file   preprocessing.c
 * @author Matheus Vieira Portela
 * @date   17/04/2014
 *
 * @brief  Implements pre-processing
 */

#include "preprocessing.h"

void preprocess(char *filename, char *output)
{
    FILE *fp;
    FILE *fout;
    char line_buffer[FILE_LINE_LENGTH];
    element_t elements;
    
    fp = file_open(filename, "r");
    fout = file_open(output, "w");
    
    while(file_read_line(fp, line_buffer) != FILE_FINISHED)
    {
        remove_comments(line_buffer);
        printf("%s", line_buffer);
        
        scan_line_elements(&elements, line_buffer);
        
        if (strcmp(elements.operation, "IF") == 0)
            printf("if directive found!\n");
        if (strcmp(elements.operation, "EQU") == 0)
            printf("equ directive found!\n");
        
        fprintf(fout, "%s", line_buffer);
    }
    
    file_close(fp);
    file_close(fout);
}

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