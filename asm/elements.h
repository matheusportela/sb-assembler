/**
 * @file   elements.h
 * @author Matheus Vieira Portela
 * @date   06/04/2014
 *
 * @brief  Declares the element struct and all necessary functions
 *
 * Elements are the basic structure for each line of the pseudo-assembly language. It
 * stores all possible elements of a line: label, operation, and operands.
 */

#ifndef _ELEMENTS_H_
#define _ELEMENTS_H_

#define ELEMENT_MAX_SIZE 100

typedef struct
{
    char label[ELEMENT_MAX_SIZE];
    char operation[ELEMENT_MAX_SIZE];
    char operand1[ELEMENT_MAX_SIZE];
    char operand2[ELEMENT_MAX_SIZE];
} element_t;

void element_init(element_t *el);
void element_clear(element_t *el);
int element_count_operands(element_t *el);
int element_has_label(element_t *el);
int element_has_operation(element_t *el);
int element_has_operand1(element_t *el);
int element_has_operand2(element_t *el);

#endif /* _ELEMENTS_H_ */