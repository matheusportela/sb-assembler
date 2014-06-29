/**
 * @file   linked_list.c
 * @author Matheus Vieira Portela
 * @author Lucas de Levy Oliveira
 * @date   07/04/2014
 *
 * @brief  Implements generic linked list functions
 */

#include "linked_list.h"

/**
 * Initialise the linked list properties and pointers.
 * @param list pointer for the previously allocated linked list pointer.
 * @param data_size size of the data element.
 * @param compare_fn pointer for a function to compare some data elements.
 * @param print_tf pointer for a function to print a data element.
 */
void list_create(list_t *list, int data_size, compare_function compare_fn,
                 print_function print_fn)
{
    if (data_size <= 0)
        error(ERROR_LINKED_LIST, "Element size must be larger than 0");
    
    if (!compare_fn)
        error(ERROR_LINKED_LIST, "Must provide a function to compare data");
    
    list->length = 0;
    list->data_size = data_size;
    list->head = NULL;
    list->tail = NULL;
    list->compare_fn = compare_fn;
    list->print_fn = print_fn;
}

/**
 * Free all linked list nodes and its data.
 * @param list pointer for the previously initialised linked list.
 */
void list_destroy(list_t *list)
{
    list_node_t *current_node;
    
    while (list->head != NULL)
    {
        current_node = list->head;
        list->head = current_node->next;

        free(current_node->data);
        free(current_node);
    }
}

/**
 * Insert some data to the beginning of the linked list.
 * @param list pointer for the previously initialised linked list.
 * @param data pointer for the previously allocated data.
 */
void list_prepend(list_t *list, void *data)
{
    list_node_t *node = malloc(sizeof(list_node_t));
    node->data = malloc(list->data_size);
    memcpy(node->data, data, list->data_size);

    node->next = list->head;
    list->head = node;

    /* first node? */
    if(!list->tail)
        list->tail = list->head;

    list->length++;
}

/**
 * Insert some data to the end of the linked list.
 * @param list pointer for the previously initialised linked list.
 * @param data pointer for the previously allocated data.
 */
void list_append(list_t *list, void *data)
{
    list_node_t *node = malloc(sizeof(list_node_t));
    node->data = malloc(list->data_size);
    node->next = NULL;
    memcpy(node->data, data, list->data_size);

    /* first node? */
    if(list->length == 0)
    {
        list->head = list->tail = node;
    }
    else
    {
        list->tail->next = node;
        list->tail = node;
    }

    list->length++;
}
 
/**
 * Return the current number of elements in the linked list.
 * @param list pointer for the previously initialised linked list.
 * @return linked list current length.
 */
int list_length(list_t *list)
{
    return list->length;
}

/**
 * Look for a node in the linked list that has the same label as the given one.
 * @param list pointer for the previously initialised linked list.
 * @param label string pointer that defines the label to be searched.
 * @return the node data pointer if there is a node with the given label, otherwise, NULL.
 */
void* list_search(list_t *list, void *label)
{
    list_node_t *current_node = list->head;
    
    while (current_node != NULL)
    {
        if (list->compare_fn(current_node->data, label))
            return current_node->data;
        
        current_node = current_node->next;
    }
    
    return NULL;
}

/**
 * Print all nodes contained in the linked list using the print function pointer given at
 * the initialisation of the linked list.
 * @param list pointer for the previously initialised linked list.
 */
void list_print(list_t *list)
{
    list_node_t *current_node = list->head;
    
    if (!list->print_fn)
    {
        printf("WARNING (linked list): No print function defined\n");
        return;
    }
    
    while (current_node != NULL)
    {
        list->print_fn(current_node->data);
        current_node = current_node->next;
    }
}