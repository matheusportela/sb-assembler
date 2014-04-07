/**
 * @file   table.c
 * @author Matheus Vieira Portela
 * @date   06/04/2014
 *
 * @brief  Implements table functions
 *
 * Tables are implemented as linked lists, searchable by its label.
 */

#include "table.h"

/**
 * Initialize table fields.
 * @param table table pointer.
 */
void table_init(table_t *table)
{
	table->head = NULL;
	table->tail = NULL;
}

/**
 * Completely free a table from the memory.
 * @param table table pointer.
 */
void table_free(table_t *table)
{
	table_node_t *table_node_ptr = table->head;
	table_node_t *next_node_ptr;
	
	while (table_node_ptr)
	{
		next_node_ptr = table_node_ptr->next_node;
		free(table_node_ptr);
		table_node_ptr = next_node_ptr;
	}
}

/**
 * Receive an already initialized table_node and insert it to the end of the table,
 * updating the tail pointer. If the table head was not defined yet, it will point to the
 * given table node.
 * @param table_ptr table pointer.
 * @param new_node_ptr table node pointer that will be inserted to the table.
 */
void table_insert_node(table_t *table_ptr, table_node_t *new_node_ptr)
{
	 /* Not even the root exists */
	if (table_ptr->head == NULL)
		table_ptr->head = new_node_ptr;
	else
		table_ptr->tail->next_node = new_node_ptr;
	
	table_ptr->tail = new_node_ptr;
}

/**
 * Add a new node to the end of a table.
 * @param table_ptr table pointer.
 * @param label string pointer.
 * @param value table node value.
 */
void table_add(table_t *table_ptr, char *label, int value)
{
	table_node_t *new_node_ptr = malloc(sizeof(table_node_t));
	
	strcpy(new_node_ptr->label, label);
	new_node_ptr->value = value;
	new_node_ptr->next_node = NULL;
	
	table_insert_node(table_ptr, new_node_ptr);
}

/**
 * Print info about all elements currently in the table
 * @param table table pointer.
 */
void table_print(table_t *table)
{
	table_node_t *table_node_ptr = table->head;
	
	printf("=== Table ===\n");
	
	while (table_node_ptr)
	{
		table_node_print(table_node_ptr);
		table_node_ptr = table_node_ptr->next_node;
	}
}

/**
 * Look for a table node with the given label and return its pointer. Return NULL if no
 * node has the label.
 * @param table pointer.
 * @param label string pointer.
 * @return The table node pointer or NULL.
 */
table_node_t* table_find(table_t *table, char *label)
{
	table_node_t *node_ptr = table->head;
	
	while (node_ptr)
	{
		if (strcmp(node_ptr->label, label) == 0)
			return node_ptr;
		
		node_ptr = node_ptr->next_node;
	}
	
	return NULL;
}

/**
 * Allocate memory and initialize a table node with the given label and value.
 * @param label string pointer.
 * @param value node value.
 * @return Allocated and initialized table node pointer.
 */
table_node_t* table_node_create(char *label, int value)
{
	table_node_t *table_node;
	
	table_node = malloc(sizeof(table_node_t));
	
	strcpy(table_node->label, label);
	table_node->value = value;
	table_node->next_node = NULL;
	
	return table_node;
}

/**
 * Print a table node info.
 * @param table_node table node pointer.
 */
void table_node_print(table_node_t *table_node)
{
	if (!table_node)
	{
		fprintf(stderr, "ERROR: Table node print: Trying to print a non allocated node\n");
		exit(-1);
	}
	
	if (!(table_node->label))
	{
		fprintf(stderr, "ERROR: Table node print: Trying to print a node without label\n");
		exit(-1);
	}
	
	printf("Label: %s\tValue: %d\n", table_node->label, table_node->value);
}