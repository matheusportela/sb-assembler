/**
 * @file   object_file.c
 * @author Matheus Vieira Portela
 * @date   18/04/2014
 *
 * @brief  Implements object file functions
 */

#include "object_file.h"

/**
 * Writes the object file to a binary file.
 * The binary file starts with a header containing the program size and the text section
 * address. 
 *  -----------------------------------------------------------------------
 * |    1 int     |        1 int         | Program size*sizeof(obj_t) bytes|
 * | Program size | Text section address |        Compiled program         |
 *  -----------------------------------------------------------------------
 * @param filename Name of the output object file.
 * @param object Object file struct.
 */
void object_file_write(char *filename, object_file_t object)
{
    FILE *fp = file_open(filename, "wb");
    
    /* Writing header */
    fwrite(&object.size, sizeof(int), 1, fp); /* Program size */
    fwrite(&object.text_section_address, sizeof(int), 1, fp); /* Text section address */
    
    /* Writing program */
    fwrite(object.program, sizeof(obj_t), object.size, fp);
    file_close(fp);
}

/**
 * Read an object binary file, saving it to an object file struct, and print on the screen.
 * @param filename Name of the input object file.
 * @param object_ptr Pointer to an object file struct.
 */
void object_file_read(char *filename, object_file_t *object_ptr)
{
    FILE *fp = fopen(filename, "rb");
    int i;
    
    /* Reading header */
    fread(&object_ptr->size, sizeof(int), 1, fp); /* Program size */
    fread(&object_ptr->text_section_address, sizeof(int), 1, fp); /* Text section address */
    
    /* Reading program */
    object_ptr->program = malloc(sizeof(obj_t)*object_ptr->size);
    fread(object_ptr->program, sizeof(obj_t), object_ptr->size, fp);
    
    /* Printing to the screen */
    printf("\n===== %s =====\n\n", filename);
    for (i = 0; i < object_ptr->size; ++i)
        printf("(addr. %d): %d\n", i, object_ptr->program[i]);
    printf("\n==========\n");
    
    fclose(fp);
}

/**
 * Initialise an object file struct.
 * @param object_ptr Pointer to an allocated object file struct.
 */
void object_file_init(object_file_t *object_ptr)
{
    object_ptr->program = NULL;
    object_ptr->offset = NULL;
    object_ptr->size = 0;
    object_ptr->text_section_address = -1;
    object_ptr->data_section_address = -1;
}

/**
 * Destroy an object file struct.
 * @param object_ptr Pointer to an allocated object file struct.
 */
void object_file_destroy(object_file_t *object_ptr)
{
    free(object_ptr->program);
}

/**
 * Add a new value to the end of an object file compiled program, increases the object
 * program size, allocate memory for it and set the offset to zero.
 * @param object_ptr Pointer to an allocated object file struct.
 * @param value Value to be added to the object file program.
 */
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

/**
 * Add a new value to the end of an object file compiled program with an offset.
 * @param object_ptr Pointer to an allocated object file struct.
 * @param value Value to be added to the object file program.
 * @param offset Offset value.
 */
void object_file_add_with_offset(object_file_t *object_ptr, obj_t value, int offset)
{
    object_file_add(object_ptr, value);
    
    object_ptr->offset[object_ptr->size - 1] = offset;
}

/**
 * Insert a value to an already existent program position. If there is a need to append a
 * new value to the end of the program, use the object_file_add function.
 * @param object_ptr Pointer to an allocated object file struct.
 * @param position Position to insert the value.
 * @param value Value to be added to the object file program.
 */
void object_file_insert(object_file_t *object_ptr, int position, obj_t value)
{
    if (position >= object_ptr->size)
        error(ERROR_OBJECT_FILE, "ERROR [object_file]: Trying to insert a value at an "
                                 "invalid position\n"
                                 "Object file size: %d\tPosition: %d\n",
                                 object_ptr->size, position);
    
    object_ptr->program[position] = value;
}

/**
 * Get the value of an existent program position.
 * @param object_ptr Pointer to an allocated object file struct.
 * @param position Position to get the value.
 */
obj_t object_file_get(object_file_t object, int position)
{
    if (position >= object.size)
        error(ERROR_OBJECT_FILE, "ERROR [object_file]: Trying to get a value at an "
                                 "invalid position\n"
                                 "Object file size: %d\tPosition: %d\n",
                                 object.size, position);
                                 
    return object.program[position];
}

/**
 * Get the offset of an existent program position.
 * @param object_ptr Pointer to an allocated object file struct.
 * @param position Position to get the value.
 */
int object_file_get_offset(object_file_t object, int position)
{
    if (position >= object.size)
        error(ERROR_OBJECT_FILE, "ERROR [object_file]: Trying to get an offset at an "
                                 "invalid position\n"
                                 "Object file size: %d\tPosition: %d\n",
                                 object.size, position);

    return object.offset[position];
}

/**
 * Print an object program.
 * @param object_ptr Pointer to an allocated object file struct.
 */
void object_file_print(object_file_t object)
{
    int i;
    
    printf("===== Object file =====\n");
    
    for (i = 0; i < object.size; ++i)
        printf("(addr. %2.2d): %d\n", i, object_file_get(object, i));
    
    printf("\n");
}