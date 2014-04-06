#include "elements.h"

/*
Init all elements fields with '\0'
*/
void element_init(element_t *el)
{
	el->label[0] = '\0';
	el->operation[0] = '\0';
	el->operand1[0] = '\0';
	el->operand2[0] = '\0';
}

/*
Clear all elements fields
*/
void element_clear(element_t *el)
{
	el->label[0] = '\0';
	el->operation[0] = '\0';
	el->operand1[0] = '\0';
	el->operand2[0] = '\0';
}

int element_count_operands(element_t *el)
{
	int num_operands = 0;

	if (element_has_operand1(el))
		++num_operands;
	if (element_has_operand2(el))
		++num_operands;
	
	return num_operands;
}

int element_has_label(element_t *el)
{
	return (el->label[0] != '\0');
}

int element_has_operation(element_t *el)
{
	return (el->operation[0] != '\0');
}

int element_has_operand1(element_t *el)
{
	return (el->operand1[0] != '\0');
}

int element_has_operand2(element_t *el)
{
	return (el->operand2[0] != '\0');
}