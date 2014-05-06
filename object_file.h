/**
 * @file   object_file.h
 * @author Matheus Vieira Portela
 * @date   18/04/2014
 *
 * @brief  Generates an object file
 */

#ifndef _OBJECT_FILE_H_
#define _OBJECT_FILE_H_

#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "file.h"

/* Object file has one byte elements */
typedef short int obj_t;

/* The object struct should have two sizes: one for the size that is being used and
 * another for the size already allocated. This will allow to remove elements by only
 * setting the value number.
 * Only necessary if object_file_remove function is defined.
 */
typedef struct
{
    obj_t *program;
    int *offset;
    int size;
} object_file_t;

void object_file_write(char *filename, object_file_t object);
void object_file_read(char *filename);
void object_file_init(object_file_t *object_ptr);
void object_file_destroy(object_file_t *object_ptr);
void object_file_add(object_file_t *object_ptr, obj_t value);
void object_file_add_with_offset(object_file_t *object_ptr, obj_t value, int offset);
void object_file_insert(object_file_t *object_ptr, int position, obj_t value);
obj_t object_file_get(object_file_t object, int position);
int object_file_get_offset(object_file_t object, int position);
void object_file_print(object_file_t object);
void object_file_test();

#endif /* _OBJECT_FILE_H_ */