/**
 * @file   linked_list.c
 * @author Matheus Vieira Portela
 * @date   07/04/2014
 *
 * @brief  Implements generic linked list functions
 */

#include "linked_list.h"

void list_create(list_t *list, int data_size, compare_function compare_fn,
                 print_function print_fn)
{
    if (data_size <= 0)
    {
        fprintf(stderr, "ERROR (linked list): Element size must be larger than 0\n");
        exit(-1);
    }
    
    if (!compare_fn)
    {
        fprintf(stderr, "ERROR (linked list): Must provide a function to compare data\n");
        exit(-1);
    }
    
    list->length = 0;
    list->data_size = data_size;
    list->head = NULL;
    list->tail = NULL;
    list->compare_fn = compare_fn;
    list->print_fn = print_fn;
}

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
 
int list_length(list_t *list)
{
    return list->length;
}

void* list_find(list_t *list, void *label)
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