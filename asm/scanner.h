/**
 * @file   scanner.h
 * @author Matheus Vieira Portela
 * @date   06/04/2014
 *
 * @brief  Defines scanner functions
 *
 * The scanner is responsible of getting a assembly line and separate all of its elements
 * into tokens with respect to the language syntax.
 */

#ifndef _SCANNER_H_
#define _SCANNER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "elements.h"
#include "error.h"

#define TOKEN_BUFFER_SIZE 100

typedef enum
{
    SCANNER_STATE_LABEL,
    SCANNER_STATE_OPERATION,
    SCANNER_STATE_OPERAND_1,
    SCANNER_STATE_OPERAND_2
} scanner_state_t;

void scan_line_elements(element_t *el, char *line);
void sanitize_token_ending(char *token);
int is_separator(char c);
int is_end_of_line(char c);
int is_label(char *token);
int is_valid_label(char *token);
int is_valid_operand(char *token);
int is_number(char *token);

#endif /* _SCANNER_H_ */