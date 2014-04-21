/**
 * @file   preprocessing.h
 * @author Matheus Vieira Portela
 * @date   17/04/2014
 *
 * @brief  Execute pre-processing in a pseudo-assembly source file
 *
 * The pre-processing evaluates only two directives: EQU (which defines text synonyms) and
 * IF blocks, that are compiled when the flag is positive (diferente than zero).
 *
 * Also, it removes all comments from the source code.
 */

#ifndef _PREPROCESSING_H_
#define _PREPROCESSING_H_

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "file.h"
#include "elements.h"
#include "scanner.h"
#include "equate_table.h"

#define NO_DIRECTIVE 0
#define DIRECTIVE_IF_NUMBER 1
#define DIRECTIVE_EQU_NUMBER 2

void preprocess(char *filename, char *output);
void preprocessing_first_pass(char *filename, hash_table_t *equate_table);
void preprocessing_second_pass(char *filename, char *output, hash_table_t *equate_table);
void remove_comments(char *line);
int detect_directive(element_t *elements, char *line);
void replace(char *str, char *old, char *new);
void to_uppercase(char *token);

#endif /* _PREPROCESSING_H_ */