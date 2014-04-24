#include "assembler.h"

void parse_arguments(int argc, char **argv, char **infile, char **prefile, char **outfile);

/**
 * Main function
 */
int main(int argc, char **argv)
{
    char *infile, *prefile, *outfile;
    
    parse_arguments(argc, argv, &infile, &prefile, &outfile);
    preprocess(infile, prefile);
    assemble(prefile, outfile);
    
    /* Print object file to the screen */
    object_file_read(outfile);
    
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