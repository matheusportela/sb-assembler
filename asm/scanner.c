/**
 * @file   scanner.c
 * @author Matheus Vieira Portela
 * @date   06/04/2014
 *
 * @brief  Implements scanner functions
 *
 * Scan elements of a line and provide utility functions, for example, to sanitize tokens
 * and check classes of characters and tokens (end of line, number etc).
 */

#include "scanner.h"

/**
 * Separate line elements in: label, operation, and operands. This is done by using a
 * state machine, that identifies which element is being extracted at the current time.
 * 
 * First, it copies the line to copy_line because it is consumed by the token extraction
 * function.
 * 
 * Start the state machine with PARSER_STATE_OPERATION, i.e., trying to extract the
 * operation token. However, if the extracted token is a LABEL, it changes to
 * PARSER_STATE_LABEL initially.
 * 
 * @param el element pointer.
 * @param line string pointer.
 */
void scan_line_elements(element_t *el, char *line)
{
    char *token;
    scanner_state_t state = SCANNER_STATE_OPERATION;
    int line_size = strlen(line);
    char copy_line[line_size];
    
    strcpy(copy_line, line);
    
    token = strtok(copy_line, " ,\t");
    while (token != NULL)
    {
        if (is_label(token))
            state = SCANNER_STATE_LABEL;
        
        sanitize_token_ending(token);
        
        switch (state)
        {
            case SCANNER_STATE_LABEL:
                strcpy(el->label, token);
                break;
            case SCANNER_STATE_OPERATION:
                strcpy(el->operation, token);
                break;
            case SCANNER_STATE_OPERAND_1:
                strcpy(el->operand1, token);
                break;
            case SCANNER_STATE_OPERAND_2:
                strcpy(el->operand2, token);
                break;
            default:
                break;
        }
        ++state;
        token = strtok(NULL, " ,\t");
    }
}

/**
 * Remove undesirable chars from the token, such as '\n', ':' etc, by changing the end of
 * the string with '\0'.
 * @param token string pointer.
 */
void sanitize_token_ending(char *token)
{
    int i;
    
    for(i = 0; token[i] != '\0'; i++)
    {
        switch (token[i])
        {
            case '\n':
            case ':':
            case -1: /* This one is weird and appears on the last word of the file */
                token[i] = '\0';
                break;
        }
    }
}

/**
 * Check whether a character is defined as a separator character.
 * @param c character to be checked.
 * @return 1 if c is a separator element or 0 otherwise.
 */
int is_separator(char c)
{
    switch (c)
    {
        case ' ':
            return 1;
    }
    
    return 0;
}

/**
 * Check whether a character is defined as a end of line character.
 * @param c character to be checked.
 * @return 1 if c is a end of line element or 0 otherwise.
 */
int is_end_of_line(char c)
{
    switch (c)
    {
        case '\n':
        case '\0':
            return 1;
    }
    
    return 0;
}

/**
 * Check whether a given token is a label, which is recognized by its last character being
 * ":".
 * @param token string pointer.
 * @return 1 if token is a label or 0 otherwise.
 */
int is_label(char *token)
{
    int token_size = strlen(token);
    char last_char = token[token_size - 1]; /* Ignoring \0 */
    return (last_char == ':');
}

/**
 * Check whether a given token is has a valid label naming, which must be composed by
 * characters 0-9, a-z, A-Z and _ (underscore).
 * Label names cannot start with a number.
 * @param token string pointer.
 * @return 1 if token is a label or 0 otherwise.
 */
int is_valid_label(char *token)
{
    char c;
    int i;
    int token_size;
    
    /* Exit immediately if starts with a number */
    if (token[0] >= '0' && token[0] <= '9')
        return 0;
    
    token_size = strlen(token) - 1; /* Ignoring the ':' character of a label */
    
    for (i = 0; i < token_size; ++i)
    {
        c = token[i];
        if (!((c >= '0' && c <= '9') ||
              (c >= 'a' && c <= 'z') ||
              (c >= 'A' && c <= 'Z') ||
              (c == '_')))
              return 0;
    }
    
    return 1;
}

/**
 * Check whether a given token is has a valid operand naming, which must be composed by
 * characters 0-9, a-z, A-Z, _ (underscore), and [ ] (square brackets for array access).
 * Label names cannot start with a number.
 * Similar implementation as is_valid_label.
 * @param token string pointer.
 * @return 1 if token is a label or 0 otherwise.
 */
int is_valid_operand(char *token)
{
    char c;
    int i;
    int token_size;
    
    /* Exit immediately if starts with a number */
    if (token[0] >= '0' && token[0] <= '9')
        return 0;
    
    token_size = strlen(token) - 1; /* Ignoring the ':' character of a label */
    
    for (i = 0; i < token_size; ++i)
    {
        c = token[i];
        if (!((c >= '0' && c <= '9') ||
              (c >= 'a' && c <= 'z') ||
              (c >= 'A' && c <= 'Z') ||
              (c == '_') ||
              (c == '[') ||
              (c == ']')))
              return 0;
    }
    
    return 1;
}

/**
 * Check whether a given token is a valid representation of a number.
 *
 * Uses the strtol function, from stdlib, which converts a string to long int. This
 * function returns the converted number and sets its second parameter, a char** pointer,
 * to the address of the last character which is evaluated during conversion. If this
 * address is not at the end of the token, it means that the conversion was not successful
 * because the token is not a valid representation of a number.
 * The last parameter is the base of the number, which may be 2, 8, 10, 16, etc. If it is
 * set to 0, it will try and get the representation from the token format.
 * More info avaliable at: http://www.cplusplus.com/reference/cstdlib/strtol/
 *
 * @param token string pointer.
 * @return 1 if token is a number or 0 otherwise.
 */
int is_number(char *token)
{
    char *end_ptr;
    int diff;
    int length;
    
    strtol(token, &end_ptr, 0);
    diff = end_ptr - token;
    length = strlen(token);
    
    return (diff == length);
}