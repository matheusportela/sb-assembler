#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

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
    
    data_t *x = list_find(&list, (void*)2);
    if (x)
        printf("x: %d, y: %d, z: %c\n", x->x, x->y, x->z);
    else
        printf("Not found\n");
        
    list_print(&list);
        
    list_destroy(&list);
    
    return 0;
}