/**
 * @file   main.c
 * @author Matheus Vieira Portela
 * @author Lucas de Levy Oliveira
 * @date   29/05/2014
 *
 * @brief  Loader for IA-32 assembly language
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "loader.h"
#include "error.h"

void parse_arguments(int argc, char **argv, char **infile, char **outfile);

int main(int argc, char **argv)
{
    char *infile, *outfile;
    
    parse_arguments(argc, argv, &infile, &outfile);
    load(infile, outfile);
    
    return 0;
}

void parse_arguments(int argc, char **argv, char **infile, char **outfile)
{
    if (argc != 3)
        error(ERROR_COMMAND_LINE, "Wrong number of arguments\n"
              "Usage: carregador <input> <output>");
    
    *infile = argv[1];
    *outfile = argv[2];
    
    printf("===== Loader =====\n");
    printf("Input file: %s\n", *infile);
    printf("Output file: %s\n", *outfile);
    printf("\n");
}
