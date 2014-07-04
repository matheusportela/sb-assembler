/**
 * @file   main.c
 * @author Matheus Vieira Portela
 * @author Lucas de Levy Oliveira
 * @date   29/05/2014
 *
 * @brief  Translator for IA-32 assembly language
 */

#include "assembler.h"
#include "translator.h"

void parse_arguments(int argc, char **argv, char **infile, char **outfile, char **opsfile);
void make_preprocess_filename(char **infile, char **prefile);
void make_binary_filename(char **infile, char **binfile);

int main(int argc, char **argv)
{
    char *infile, *outfile;
    char *prefile, *binfile, *opsfile;
    
    parse_arguments(argc, argv, &infile, &outfile, &opsfile);
    make_preprocess_filename(&infile, &prefile);
    make_binary_filename(&infile, &binfile);
    preprocess(infile, prefile);
    assemble(prefile, binfile);
    translate(binfile, outfile, opsfile);
    
    free(prefile);
    free(binfile);
    
    return 0;
}

void parse_arguments(int argc, char **argv, char **infile, char **outfile, char **opsfile)
{
    if (argc != 4)
        error(ERROR_COMMAND_LINE, "Wrong number of arguments\n"
              "Usage: tradutor <input> <output>");
    
    *infile = argv[1];
    *outfile = argv[2];
    *opsfile = argv[3];
    
    
    printf("===== Parsing arguments =====\n");
    printf("Input file: %s\n", *infile);
    printf("Output file: %s\n", *outfile);
    printf("Opcodes file: %s\n", *opsfile);
    printf("\n");
}

void make_preprocess_filename(char **infile, char **prefile)
{
    *prefile = malloc(sizeof(char)*(strlen(*infile) + 4));
    strcpy(*prefile, *infile);
    strcat(*prefile, ".pre");
    printf("Preprocess filename: %s\n", *prefile);
}

void make_binary_filename(char **infile, char **binfile)
{
    *binfile = malloc(sizeof(char)*(strlen(*infile) + 4));
    strcpy(*binfile, *infile);
    strcat(*binfile, ".bin");
    printf("Binary filename: %s\n", *binfile);
}
