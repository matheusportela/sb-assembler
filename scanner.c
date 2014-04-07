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

/*
Separate line elements in: label, operation, operands, and comments.
First, it copies the line to copy_line because it is consumed by strsep.
Start the state machine with PARSER_STATE_OPERATION, i.e., trying to extract the operation
token. However, if the extracted token is a LABEL, it changes to PARSER_STATE_LABEL
initially.
*/
void scan_line_elements(element_t *el, char *line, int line_size)
{
	int debug = 0;
	char *token;
	scanner_state_t state = SCANNER_STATE_OPERATION;
	char copy_line[line_size];
	char *copy_line_ptr;
	
	strcpy(copy_line, line);
	copy_line_ptr = copy_line;
	
	token = strtok(copy_line," ,\t");
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
				/* TODO: Ignore? (May be a comment) */
				fprintf(stderr, "ERROR: In get_line_elements: Not a valid state %d Last token: %s\n",
						state, token);
				exit(-1);
		}
		++state;
		token = strtok (NULL, " ,\t");
	}
	
	if (debug)
		printf("\n");
}

/*
Extract one token from the line, starting at the index initial_position.
Returns either the last position of the token when there is still more words to be
extracted or -1 when the end of the line was reached.
*/
int get_token(char **token, char *line, int pos)
{
	int i; /* Line iterator */
	int j; /* Buffer iterator */
	char buffer[TOKEN_BUFFER_SIZE]; /* Token buffer */
	
	for (i = pos, j = 0; !is_end_of_line(line[i]); i++, j++)
	{
		if (is_separator(line[i]))
			break;
			
		buffer[j] = line[i];
	}
	buffer[j] = '\0';
	
	/* Token now points to the buffered string */
	*token = buffer;
	
	if (is_end_of_line(line[i]))
		return -1;
	
	return i+1; /* Jump the last char because it was already analyzed */
}

/*
Remove undesirable chars from the token, such as '\n', ':' etc, by changing the end of the
string with '\0'
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

/*
Check whether a character is defined as a separator character
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

/*
Check whether a character is defined as a end of line character
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

/*
Check whether a given token is a label, which is recognized by its last character being
":"
*/
int is_label(char *token)
{
	int token_size = strlen(token);
	char last_char = token[token_size - 1]; /* Ignoring \0 */
	return (last_char == ':');
}

/*
Check whether a given token is a number
*/
int is_number(char *token)
{
	int i;
	
	for(i = 0; token[i] != '\0'; ++i)
	{
		if (!(token[i] >= '0' && token[i] <= '9'))
			return 0;
	}
	return 1;
}