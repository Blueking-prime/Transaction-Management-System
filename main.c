#include "main.h"

enum CommandType {
    INVALID,
    NEW,
    DEPOSIT,
    WITHDRAW,
    CHECK,
    LIQUIDATE
};

/**
 * Returns the corresponding enum value of type CommandType based on the given command string.
 *
 * @param command The command string to be checked.
 * @return The enum value of type CommandType based on the comparison of the command string.
 */
enum CommandType get_command_type(const char* command) {
    if (strcmp(command, "new") == 0) {
        return NEW;
    } else if (strcmp(command, "deposit") == 0) {
        return DEPOSIT;
    } else if (strcmp(command, "withdraw") == 0) {
        return WITHDRAW;
    } else if (strcmp(command, "check") == 0) {
        return CHECK;
    } else if (strcmp(command, "liquidate") == 0) {
        return LIQUIDATE;
    } else {
        return INVALID;
    }
}

int main(void) {
    int ret;
    char **command = NULL, *input = NULL, *ppath = NULL;

    while (1)
    {
        // Read user input
        input = input_prompt();
        ppath = format_input(input);
        if (ppath[0] == '\n')
        {
            free(ppath);
            continue;
        }

        // Checks if user used exit command
        if (strncmp(ppath, "exit", strlen("exit")) == 0)
            break;

        // Checks if user used help command
        if (strncmp(ppath, "help", strlen("help")) == 0){
            display_help_generic();
            continue;
        }

        // Breaks the arguments
        command = arg_split(ppath);
        if (!command[1]){
            puts("Insufficient arguments");
            continue;
        }

        //todo: Converts command to lowercase
        // for ( ; *command[0]; ++command[0]) *command[0] = tolower(*command[0]);

        // todo: Add specialized error codes
        // Create a new Account
        switch (get_command_type(command[0])) {
            case NEW:
                if (!command[2] || !command[3] || !command[4]){
                    puts("Not enough arguments\n" \
                    "Usage: new <first name> <last name> <bvn> <pin>");
                    continue;
                }
                ret = new(command[1], command[2], command[3], command[4]);
                if (!ret){
                    puts("Something went wrong, please try again");
                    continue;
                }
                printf("Your new account number is %010d\n", ret);
                break;
            case DEPOSIT:
                if (!command[2]){
                    puts("Not enough arguments\n" \
                    "Usage: deposit <account number> <amount>");
                    continue;
                }
                ret = deposit(command[1], command[2]);
                if (ret){
                    puts("Something went wrong, please try again");
                    continue;
                }
                printf("You deposited %s\n", command[2]);
                break;
            case WITHDRAW:
                if (!command[2] || !command[3]){
                    puts("Not enough arguments\n" \
                    "Usage: withdraw <account number> <pin> <amount>");
                    continue;
                }
                ret = withdraw(command[1], command[2], command[3]);
                if (ret){
                    puts("Something went wrong, please try again");
                    continue;
                }
                printf("You withdrew %s\n", command[3]);
                break;
            case CHECK:
                if (!command[2]){
                    puts("Not enough arguments\n" \
                    "Usage: check <account number> <pin>");
                    continue;
                }
                ret = check(command[1], command[2]);
                if (ret){
                    puts("Something went wrong, please try again");
                    continue;
                }
                break;
            case LIQUIDATE:
                if (!command[2] || !command[3]){
                    puts("Not enough arguments\n" \
                    "Usage: liquidate <account number> <pin> <bvn>");
                    continue;
                }
                ret = liquidate(command[1], command[2], command[3]);
                if (ret){
                    puts("Something went wrong, please try again");
                    continue;
                }
                puts("You've deleted your account successfully");
                break;
            default:
                puts("Invalid command");
                break;
        }

        free(ppath);
        free(command);
    }
}
