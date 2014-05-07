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

/*
 * An object file struct contains five sections:
 * - program: Contains the compiled program, which will be written to the object file.
 * - offset: Offset, used for array accessing.
 * - size: Current program size, in words.
 * - text_section_address: Start of the text section.
 * - data_section_address: Start of the data section.
 */
typedef struct
{
    obj_t *program;
    int *offset;
    int size;
    int text_section_address;
    int data_section_address;
} object_file_t;

void object_file_write(char *filename, object_file_t object);
void object_file_read(char *filename, object_file_t *object_ptr);
void object_file_init(object_file_t *object_ptr);
void object_file_destroy(object_file_t *object_ptr);
void object_file_add(object_file_t *object_ptr, obj_t value);
void object_file_add_with_offset(object_file_t *object_ptr, obj_t value, int offset);
void object_file_insert(object_file_t *object_ptr, int position, obj_t value);
obj_t object_file_get(object_file_t object, int position);
int object_file_get_offset(object_file_t object, int position);
void object_file_print(object_file_t object);

#endif /* _OBJECT_FILE_H_ */