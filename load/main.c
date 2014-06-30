/**
 * @file   main.c
 * @author Matheus Vieira Portela
 * @author Lucas de Levy Oliveira
 * @date   29/05/2014
 *
 * @brief  Loader for IA-32 assembly language
 */

void parse_arguments(int argc, char **argv, char **infile);
void make_output_filename(char **infile, char **outfile);


int main(int argc, char **argv)
{
    char *infile, *outfile;
    
    parse_arguments(argc, argv, &infile);
    make_output_filename(&infile, &outfile);
    load(infile, outfile);
    free(outfile);
    
    return 0;
}

void parse_arguments(int argc, char **argv, char **infile, char **outfile)
{
    if (argc != 2)
        error(ERROR_COMMAND_LINE, "Wrong number of arguments\n"
              "Usage: carregador <input>");
    
    *infile = argv[1];
    
    printf("===== Parsing arguments =====\n");
    printf("Input file: %s\n", *infile);
    printf("\n");
}

void make_output_filename(char **infile, char **outfile)
{
    *outfile = malloc(sizeof(char)*(strlen(*infile) + 4));
    strcpy(*outfile, *infile);
    strcat(*outfile, ".out");
    printf("Output filename: %s\n", *outfile);
}