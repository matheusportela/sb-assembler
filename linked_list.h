/**
 * @file   linked_list.h
 * @author Matheus Vieira Portela
 * @date   07/04/2014
 *
 * @brief  Declares generic linked list functions
 *
 * Following implementation suggested at
 * http://pseudomuto.com/development/2013/05/02/implementing-a-generic-linked-list-in-c.html
 *
 * Example usage
 *
    typedef struct
    {
        int x;
        int y;
        char z;
    } data_t;

    int compare(data_t *data, int key)
    {
        if (data->x == key)
            return 1;
        return 0;
    }

    void print(data_t *data)
    {
        printf("x: %d\ty: %d\tz: %c\n", data->x, data->y, data->z);
    }

    int main()
    {
        list_t list;
        data_t a, b, c;

        a.x = 0;
        a.y = 1;
        a.z = 'a';

        b.x = 2;
        b.y = 3;
        b.z = 'b';

        c.x = 4;
        c.y = 5;
        c.z = 'c';

        list_create(&list, sizeof(data_t), (void*)compare, (void*)print);
        list_prepend(&list, &a);
        list_prepend(&list, &b);
        list_prepend(&list, &c);

        printf("Length: %d\n", list_length(&list));

        data_t *x = list_search(&list, (void*)2);
        if (x)
            printf("x: %d, y: %d, z: %c\n", x->x, x->y, x->z);
        else
            printf("Not found\n");
    
        list_print(&list);
    
        list_destroy(&list);

        return 0;
    }
 */

#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"

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

typedef struct
{
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
void list_append(list_t *list, void *data);
int list_length(list_t *list);
void* list_search(list_t *list, void *label);
void list_print(list_t *list);

#endif /* _LINKED_LIST_H_ */