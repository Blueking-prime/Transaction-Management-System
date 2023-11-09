#include "main.h"

/**
* input_prompt - Requests user input
*
* Return: input string.
*/
char *input_prompt(void)
{
	size_t input_size;
	int stat;
	char *input = NULL;


	if (isatty(STDIN_FILENO) == 1)
	{
		printf("($) ");
	}
	stat = getline(&input, &input_size, stdin);
	if (stat == -1)
	{
		free(input);
		if (isatty(STDIN_FILENO) == 1)
		{
			putchar('\n');
		}
		exit(0);
	}

	return (input);
	free(input);
}


/**
* format_input - removes newline character from input
*
* @command: The users input string
*
* Return: input string.
*/
char *format_input(char *command)
{
	int i;

	for (i = 1; command[i] != '\0'; i++)
	{
		if (command[i] == '\n')
		{
			command[i] = command[i + 1];
		}
	}

	return (command);
}


/**
* arg_split - Parses the users input
*
* @input: input string
*
* Return: List of arguments
*/
char **arg_split(char *input)
{
    char **arg_list;
    int i, j = 0;
    char *token;
    char *saveptr;
    int max_tokens = 10; // Set a maximum limit for the number of tokens

    i = strlen(input) + 1;

    arg_list = malloc((max_tokens * sizeof(char)) + sizeof(NULL) + (7 * sizeof(char))); // Allocate memory based on the maximum limit
    if (arg_list == NULL)
    {
        free(arg_list);
        exit(errno);
    }

    token = strtok_r(input, " ", &saveptr);
    while (token != NULL && j < max_tokens) // Stop parsing if the maximum limit is reached
    {
        arg_list[j] = token;
        token = strtok_r(NULL, " ", &saveptr);
        j++;
    }

    return (arg_list);
}


/**
* display_help_generic - Displays the help menu for the Transaction Management system
*
* This function prints out the available operations and their corresponding commands.
* It provides a brief explanation of each command and its required arguments.
*/
void display_help_generic(void){
    printf("This is a Transaction Management system\n");
    printf("It allows you to perform multiple operations including:\n");
    printf("- Create a new account:\n");
    printf("    new <first name> <last name> <bvn> <pin>\n\n");
    printf("- Deposit money into an account:\n");
    printf("    deposit <account number> <amount>\n\n");
    printf("- Withdraw money from an account:\n");
    printf("    withdraw <account number> <pin> <amount>\n\n");
    printf("- Check account balance:\n");
    printf("    check <account number> <pin>\n\n");
    printf("Close an account:\n");
    printf("    liquidate <account number> <pin> <bvn>");
}
