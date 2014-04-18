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

#define NO_DIRECTIVE 0
#define DIRECTIVE_IF_NUMBER 1
#define DIRECTIVE_EQU_NUMBER 2

#include <stdio.h>
#include <string.h>
#include "file.h"
#include "elements.h"
#include "scanner.h"

void preprocess(char *filename, char *output);
void remove_comments(char *line);
int detect_directive(element_t *elements, char *line);

#endif /* _PREPROCESSING_H_ */