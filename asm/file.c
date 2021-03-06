/**
 * @file   file.c
 * @author Matheus Vieira Portela
 * @author Lucas de Levy Oliveira
 * @date   06/04/2014
 *
 * @brief  Implements utility functions over files
 */

#include "file.h"

/**
 * Opens a file in reading mode and check for erros.
 * @param filename the name of the file to be opened.
 * @param mode the opening mode (e.g. 'r' for read, 'w' for write, or 'a' for append)
 * @return pointer to the opened file struct.
 */
FILE* file_open(char *filename, char *mode)
{
    FILE *fp = fopen(filename, mode);
    
    if (!fp)
        error(ERROR_FILE, "Cannot open file %s", filename);
    
    return fp;
}

/**
 * Prints all lines of a file, preceded by its number.
 * @param fp opened file pointer.
 */
void file_print(FILE *fp)
{
    char line[FILE_LINE_LENGTH];
    int i;
    
    for(i = 0; file_read_line(fp, line) != FILE_FINISHED; ++i)
        printf("%d: %s", i, line);
        
    printf("\n");
}

/**
 * Closes a file
 * @param fp opened file pointer.
 */
void file_close(FILE *fp)
{
    fclose(fp);
}

/**
 * Read only one line of the file, of at most FILE_LINE_LENGTH chars, that ends with '\n'.
 * Returns the line size when it was successfully read and FILE_FINISHED if it has reached
 * the end of file.
 * @param fp opened file pointer.
 * @param line line vector to store the characters.
 * @return line size of FILE_FINISHED
 */
int file_read_line(FILE *fp, char line[])
{
    int i;
    char c = ' ';
    
    if (feof(fp))
        return FILE_FINISHED;
    
    for (i = 0; (c != '\n') && (c != EOF); i++)
    {
        if (i >= FILE_LINE_LENGTH)
            error(ERROR_FILE, "Trying to read a line larger than the line buffer "
                              "size");
        
        c = getc(fp);
        line[i] = c;
    }
    line[i] = '\0';
    
    return i;
}