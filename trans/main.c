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

void parse_arguments(int argc, char **argv, char **infile, char **outfile);
void make_preprocess_filename(char **infile, char **prefile);
void make_binary_filename(char **infile, char **binfile);
void make_opcodes_filename(char **infile, char **opfile);

/**
 * Main function. Parse the arguments, preprocess the input file and assemble the
 * preprocessed file, generating an object file.
 */
int main(int argc, char **argv)
{
    char *infile, *outfile;
    char *prefile, *binfile, *opfile;
    
    parse_arguments(argc, argv, &infile, &outfile);
    make_preprocess_filename(&infile, &prefile);
    make_binary_filename(&infile, &binfile);
    make_opcodes_filename(&outfile, &opfile);
    preprocess(infile, prefile);
    assemble(prefile, binfile);
    translate(binfile, outfile, opfile);
    
    free(prefile);
    free(binfile);
    free(opfile);
    
    return 0;
}

/**
 * Get arguments from command line
 * @param argc number of arguments
 * @param argv command line arguments
 * @param infile input file name with code in assembly
 * @param outfile output file name for object code
 */
void parse_arguments(int argc, char **argv, char **infile, char **outfile)
{
    if (argc != 3)
        error(ERROR_COMMAND_LINE, "Wrong number of arguments\n"
              "Usage: tradutor <input> <output>");
    
    *infile = argv[1];
    *outfile = argv[2];
    
    
    printf("===== Parsing arguments =====\n");
    printf("Input file: %s\n", *infile);
    printf("Output file: %s\n", *outfile);
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

void make_opcodes_filename(char **infile, char **opfile)
{
    *opfile = malloc(sizeof(char)*(strlen(*infile) + 4));
    strcpy(*opfile, *infile);
    strcat(*opfile, ".ops");
    printf("Opcodes filename: %s\n", *opfile);
}
