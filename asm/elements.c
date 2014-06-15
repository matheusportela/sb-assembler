/**
 * @file   elements.c
 * @author Matheus Vieira Portela
 * @author Lucas de Levy Oliveira
 * @date   06/04/2014
 *
 * @brief  Implements elements routines
 *
 * All fields in the element struct is implemented as a string of constant length. To make
 * comparisons easier, strings are said to be empty when the first character is '\0'.
 *
 * Example: element_has_label(el) returns 0 if the label is '\0abc' and 1 if it is 'abc'
 */

#include "elements.h"

/**
 * Init all elements fields with '\0'
 * @param el element pointer.
 */
void element_init(element_t *el)
{
    el->label[0] = '\0';
    el->operation[0] = '\0';
    el->operand1[0] = '\0';
    el->operand2[0] = '\0';
}

/**
 * Clear all elements fields
 * @param el element pointer.
 */
void element_clear(element_t *el)
{
    el->label[0] = '\0';
    el->operation[0] = '\0';
    el->operand1[0] = '\0';
    el->operand2[0] = '\0';
}

/**
 * Count the number of non-empty operands of an element.
 * @param el element pointer.
 * @return number of non-empty operands.
 */
int element_count_operands(element_t *el)
{
    int num_operands = 0;

    if (element_has_operand1(el))
        ++num_operands;
    if (element_has_operand2(el))
        ++num_operands;
    
    return num_operands;
}

/**
 * Check whether the label field is filled or empty.
 * @param el element pointer.
 * @return 1 if label is non-empty or 0 otherwise.
 */
int element_has_label(element_t *el)
{
    return (el->label[0] != '\0');
}

/**
 * Check whether the operation field is filled or empty.
 * @param el element pointer.
 * @return 1 if operation is non-empty or 0 otherwise.
 */
int element_has_operation(element_t *el)
{
    return (el->operation[0] != '\0');
}

/**
 * Check whether the operand1 field is filled or empty.
 * @param el element pointer.
 * @return 1 if operand1 is non-empty or 0 otherwise.
 */
int element_has_operand1(element_t *el)
{
    return (el->operand1[0] != '\0');
}

/**
 * Check whether the operand2 field is filled or empty.
 * @param el element pointer.
 * @return 1 if operand2 is non-empty or 0 otherwise.
 */
int element_has_operand2(element_t *el)
{
    return (el->operand2[0] != '\0');
}