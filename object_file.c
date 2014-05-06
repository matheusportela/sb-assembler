/**
 * @file   object_file.c
 * @author Matheus Vieira Portela
 * @date   18/04/2014
 *
 * @brief  Implements object file functions
 */

#include "object_file.h"

void object_file_write(char *filename, object_file_t object)
{
    FILE *fp = file_open(filename, "wb");
    fwrite(&object.size, sizeof(int), 1, fp); /* First word is the program size */
    fwrite(object.program, sizeof(obj_t), object.size, fp);
    file_close(fp);
}

void object_file_read(char *filename)
{
    FILE *fp = file_open(filename, "rb");
    int program_size;
    obj_t *buffer;
    int i;
    
    fread(&program_size, sizeof(int), 1, fp); /* First word is the program size */
    
    buffer = malloc(sizeof(obj_t)*program_size);
    fread(buffer, sizeof(obj_t), program_size, fp);
    
    printf("\n===== %s =====\n\n", filename);
    for (i = 0; i < program_size; ++i)
        printf("(addr. %2.2d): %d\n", i, buffer[i]);
    printf("\n==========\n");
    
    file_close(fp);
    free(buffer);
}

void object_file_init(object_file_t *object_ptr)
{
    object_ptr->program = NULL;
    object_ptr->size = 0;
}

void object_file_destroy(object_file_t *object_ptr)
{
    free(object_ptr->program);
}

void object_file_add(object_file_t *object_ptr, obj_t value)
{
    ++object_ptr->size;
    
    /* Not allocated yet */
    if (object_ptr->size == 1)
    {
        object_ptr->program = malloc(sizeof(obj_t));
        object_ptr->offset = malloc(sizeof(int));
    }
    else
    {
        object_ptr->program = realloc(object_ptr->program, sizeof(obj_t)*object_ptr->size);
        object_ptr->offset = realloc(object_ptr->offset, sizeof(int)*object_ptr->size);
    }
        
    object_ptr->program[object_ptr->size - 1] = value;
    object_ptr->offset[object_ptr->size - 1] = 0;
}

void object_file_add_with_offset(object_file_t *object_ptr, obj_t value, int offset)
{
    object_file_add(object_ptr, value);
    
    object_ptr->offset[object_ptr->size - 1] = offset;
}

void object_file_insert(object_file_t *object_ptr, int position, obj_t value)
{
    if (position >= object_ptr->size)
        error(ERROR_OBJECT_FILE, "ERROR [object_file]: Trying to insert element at an "
                                 "invalid position\n"
                                 "Object file size: %d\tPosition: %d\n",
                                 object_ptr->size, position);
    
    object_ptr->program[position] = value;
}

obj_t object_file_get(object_file_t object, int position)
{
    return object.program[position];
}

int object_file_get_offset(object_file_t object, int position)
{
    return object.offset[position];
}

void object_file_print(object_file_t object)
{
    int i;
    
    printf("===== Object file =====\n");
    
    for (i = 0; i < object.size; ++i)
        printf("(addr. %2.2d): %d\n", i, object_file_get(object, i));
    
    printf("\n");
}

void object_file_test()
{
    object_file_t obj;
    object_file_init(&obj);
    object_file_add(&obj, 1);
    object_file_add(&obj, 2);
    object_file_add(&obj, 3);
    object_file_insert(&obj, 1, 4);
    
    printf("Object file test\n\n");
    
    printf("Object file:\n");
    object_file_print(obj);

    object_file_write("object.txt", obj);
    object_file_read("object.txt");

    object_file_destroy(&obj);
    
    printf("\nFinished test\n");
}