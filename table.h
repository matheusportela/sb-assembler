#ifndef _TABLE_H_
#define _TABLE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_NODE_LABEL_SIZE 30

struct table_node_struct
{
	char label[TABLE_NODE_LABEL_SIZE];
	int value;
	struct table_node_struct *next_node;
};
typedef struct table_node_struct table_node_t;

typedef struct
{
	table_node_t *head;
	table_node_t *tail;
} table_t;

void table_init(table_t *table);
void table_free(table_t *table);
void table_insert_node(table_t *table_ptr, table_node_t *new_node_ptr);
void table_add(table_t *table, char *label, int value);
void table_print(table_t *table);
table_node_t* table_find(table_t *table, char *label);

table_node_t* table_node_create(char *label, int value);
void table_node_print(table_node_t *table_node);

#endif /* _TABLE_H_ */