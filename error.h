/**
 * @file   error.h
 * @author Matheus Vieira Portela
 * @date   21/04/2014
 *
 * @brief  Declares error printing functions
 *
 * Following the implementation available at
 * http://www.ozzu.com/cpp-tutorials/tutorial-writing-custom-printf-wrapper-function-t89166.html
 */

#ifndef _ERROR_H_
#define _ERROR_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

typedef enum
{
    ERROR_COMMAND_LINE=1,
    ERROR_FILE,
    ERROR_OBJECT_FILE,
    ERROR_LINKED_LIST,
    ERROR_SCANNER,
    ERROR_LEXICAL,
    ERROR_SYNTACTIC,
    ERROR_SEMANTIC,
    ERROR_MEMORY,
} error_t;

void error(error_t error_type, const char* format, ...);
void error_at_line(error_t error_type, int line_number, const char* format, ...);
void print_error_type(error_t error_type);

#endif /* _ERROR_H_ */