/**
 * @file   file.h
 * @author Matheus Vieira Portela
 * @date   06/04/2014
 *
 * @brief  Declares utility functions over files
 *
 * These functions offer utilities over files, for instance, error checking and line
 * reading without defining a number of bytes to be read.
 */

#ifndef _FILE_H_
#define _FILE_H_

#include <stdio.h>
#include <stdlib.h>
#include "error.h"

#define FILE_LINE_LENGTH 500
#define FILE_FINISHED -1

FILE* file_open(char *filename, char *mode);
void file_print(FILE *fp);
void file_close(FILE *fp);
int file_read_line(FILE *fp, char line[]);

#endif /* _FILE_H_ */