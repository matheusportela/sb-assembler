/**
 * @file   file.c
 * @author Matheus Vieira Portela
 * @date   06/04/2014
 *
 * @brief  Implements utility functions over files
 */

#include "file.h"

/*
Opens a file and check for correctness
*/
FILE* file_open(char *filename)
{
	FILE *fp = fopen(filename, "r");
	
	if (!fp)
	{
		fprintf(stderr, "ERROR: Cannot open file %s\n", filename);
		exit(-1);
	}
	
	return fp;
}

/*
Prints all lines of a file
*/
void file_print(FILE *fp)
{
	char line[FILE_LINE_LENGTH];
	int i;
	
	for(i = 0; file_read_line(fp, line) != FILE_FINISHED; ++i)
	{
		printf("%d: %s", i, line);
	}
	printf("\n");
}

/*
Closes a file
*/
void file_close(FILE *fp)
{
	fclose(fp);
}

/*
Read only one line of the file, which ends with '\n'.
Returns the line size when it was successfully read and FILE_LINE_LENGTH if it reached the
end of file.
*/
int file_read_line(FILE *fp, char line[])
{
	int i;
	char c = ' ';
	
	if (feof(fp))
		return FILE_FINISHED;
	
	for (i = 0; (c != '\n') && (c != EOF); i++)
	{
		if (i >= FILE_LINE_LENGTH)
			fprintf(stderr, "WARNING: Trying to read a line larger than the line buffer "
							 "size\n");
		
		c = getc(fp);
		line[i] = c;
	}
	line[i] = '\0';
	
	return i;
}