/**
 * @file   directives_table.h
 * @author Matheus Vieira Portela
 * @date   06/04/2014
 *
 * @brief  Declares the directives table
 */

#ifndef _DIRECTIVES_TABLE_H_
#define _DIRECTIVES_TABLE_H_

#include "hash_table.h"

typedef struct
{
    int size;
    int (*function)(void);
} directive_t;

void directives_table_init(hash_table_t *directives_table);
void directives_table_add(hash_table_t *directives_table, char *label, int size, int (*function)(void));

int directives_run_space();
int directives_run_const();

#endif /* _DIRECTIVES_TABLE_H_ */