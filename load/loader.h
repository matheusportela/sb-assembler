#ifndef __LOADER_H__
#define __LOADER_H__

void assemble(char *filename);
int read_program(char *filename, unsigned char *code);
void load(char *infile, char *outfile);

#endif /* __LOADER_H__ */
