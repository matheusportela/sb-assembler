#include "table.h"

/*
Initialize table variables
*/
void table_init(table_t *table)
{
	table->head = NULL;
	table->tail = NULL;
}

/*
Free memory of a table
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

/*
Receive an already initialized table_node and insert it to the end of the table
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

/*
Add a new node to the end of the table
*/
void table_add(table_t *table_ptr, char *label, int value)
{
	table_node_t *new_node_ptr = malloc(sizeof(table_node_t));
	
	strcpy(new_node_ptr->label, label);
	new_node_ptr->value = value;
	new_node_ptr->next_node = NULL;
	
	table_insert_node(table_ptr, new_node_ptr);
}

/*
Print info about all elements currently in the table
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

/*
Allocate memory and initialize a table_node_t variable
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

/*
Print a table node info
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