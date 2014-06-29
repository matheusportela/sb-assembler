/**
 * @file   main.c
 * @author Matheus Vieira Portela
 * @author Lucas de Levy Oliveira
 * @date   01/05/2014
 *
 * @brief  Assembler for didactic assembly language
 */

#include "translator.h"

void parse_arguments(int argc, char **argv, char **infile, char **prefile, char **outfile);

/**
 * Main function. Parse the arguments, preprocess the input file and assemble the
 * preprocessed file, generating an object file.
 */
int main(int argc, char **argv)
{
    char *infile, *prefile, *outfile;
    
    parse_arguments(argc, argv, &infile, &prefile, &outfile);
    preprocess(infile, prefile);
    translate(prefile, outfile);
    
    return 0;
}

/**
 * Get arguments from command line
 * @param argc number of arguments
 * @param argv command line arguments
 * @param infile input file name with code in assembly
 * @param prefile output file name for preprocessed code
 * @param outfile output file name for object code
 */
void parse_arguments(int argc, char **argv, char **infile, char **prefile, char **outfile)
{
    if (argc != 4)
        error(ERROR_COMMAND_LINE, "Wrong number of arguments\n"
              "Usage: assembler <input> <preprocessing> <output>");
    
    *infile = argv[1];
    *prefile = argv[2];
    *outfile = argv[3];
    
    printf("===== Parsing arguments =====\n");
    printf("Input file: %s\n", *infile);
    printf("Pre-processing file: %s\n", *prefile);
    printf("Output file: %s\n", *outfile);
    printf("\n");
}