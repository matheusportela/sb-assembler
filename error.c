/**
 * @file   error.c
 * @author Matheus Vieira Portela
 * @date   21/04/2014
 *
 * @brief  Implements error printing functions
 */

#include "error.h"

/**
 * Print an error message with proper format and exit the execution of the program with
 * the defined error type number.
 * @param error_type Error type id number.
 * @param format Error message to output on the screen.
 */
void error(error_t error_type, const char* format, ...)
{
    va_list args;
    
    fprintf(stderr, "ERROR [");
    print_error_type(error_type);
    fprintf(stderr, "] ");
    
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");
    
    exit(error_type);
}

/**
 * Print an error message with proper format, including the line number and exit the
 * execution of the program with the defined error type number.
 * @param error_type Error type id number.
 * @param line_number Line number at which the error occurred.
 * @param format Error message to output on the screen.
 */
void error_at_line(error_t error_type, int line_number, const char* format, ...)
{
    va_list args;
    
    fprintf(stderr, "ERROR [");
    print_error_type(error_type);
    fprintf(stderr, "] line %d: ", line_number);
    
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");
    
    exit(error_type);
}

/**
 * Print error type name on the screen.
 * @param error_type Error type id number.
 */
void print_error_type(error_t error_type)
{
    switch (error_type)
    {
        case ERROR_COMMAND_LINE:
            fprintf(stderr, "command line");
            break;
        case ERROR_FILE:
            fprintf(stderr, "file");
            break;
        case ERROR_OBJECT_FILE:
            fprintf(stderr, "object file");
            break;
        case ERROR_LINKED_LIST:
            fprintf(stderr, "linked list");
            break;
        case ERROR_SCANNER:
            fprintf(stderr, "scanner");
            break;
        case ERROR_LEXICAL:
            fprintf(stderr, "lexical");
            break;
        case ERROR_SYNTACTIC:
            fprintf(stderr, "syntactic");
            break;
        case ERROR_SEMANTIC:
            fprintf(stderr, "semantic");
            break;
        case ERROR_MEMORY:
            fprintf(stderr, "memory");
            break;
    }
}