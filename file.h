#ifndef _FILE_H_
#define _FILE_H_

#include <stdio.h>
#include <stdlib.h>

#define FILE_LINE_LENGTH 120
#define FILE_FINISHED -1

FILE* file_open(char *filename);
void file_print(FILE *fp);
void file_close(FILE *fp);
int file_read_line(FILE *fp, char line[]);

#endif /* _FILE_H_ */