/**
 * @file   linked_list.h
 * @author Matheus Vieira Portela
 * @date   06/04/2014
 *
 * @brief  Declares generic linked list functions
 *
 * Following implementation suggested at
 * http://pseudomuto.com/development/2013/05/02/implementing-a-generic-linked-list-in-c.html
 */

#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* a common function used to compare data objects */
typedef int (*compare_function)(void *, void *);

/* a common function used to print data objects */
typedef void (*print_function)(void *);

struct list_node_struct
{
    void *data;
    struct list_node_struct *next;
};
typedef struct list_node_struct list_node_t;

typedef struct {
  int length;
  int data_size;
  list_node_t *head;
  list_node_t *tail;
  compare_function compare_fn;
  print_function print_fn;
} list_t;

void list_create(list_t *list, int data_size, compare_function compare_fn,
                 print_function print_fn);
void list_destroy(list_t *list);
 
void list_prepend(list_t *list, void *data);
/* TODO: define a append function */
int list_length(list_t *list);
void* list_find(list_t *list, void *label);
void list_print(list_t *list);

#endif /* _LINKED_LIST_H_ */