#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lists.h"

#define INPUT_BUFFER_SIZE 256
#define INPUT_ARG_MAX_NUM 5
#define DELIM " \n"


/* A standard template for error messages */
void error(const char *msg) {
    fprintf(stderr, "Error: %s\n", msg);
}

/* 
 * Read and process buxfer commands
 */
int process_args(int cmd_argc, char **cmd_argv, Group **group_list_addr) {
    Group *group_list = *group_list_addr; 
    //printf("%p\n", *group_list_addr);
    //printf("%s\n", cmd_argv[1]);
    Group *g;

    if (cmd_argc <= 0) {
        return 0;
    } else if (strcmp(cmd_argv[0], "quit") == 0 && cmd_argc == 1) {
        return -1;
        
    } else if (strcmp(cmd_argv[0], "add_group") == 0 && cmd_argc == 2) {
        if (add_group(group_list_addr, cmd_argv[1]) == -1) {
            error("Group already exists");
        }
        
    } else if (strcmp(cmd_argv[0], "list_groups") == 0 && cmd_argc == 1) {
        list_groups(group_list);
        
    } else if (strcmp(cmd_argv[0], "add_user") == 0 && cmd_argc == 3) {
        if ((g = find_group(group_list, cmd_argv[1])) == NULL) {
            error("Group does not exist");
        } else {
            if (add_user(g, cmd_argv[2]) == -1) {
                error("User already exists");
            }
        }
        
    } else if (strcmp(cmd_argv[0], "remove_user") == 0 && cmd_argc == 3) {
        if ((g = find_group(group_list, cmd_argv[1])) == NULL) {
            error("Group does not exist");
        } else {
            if (remove_user(g, cmd_argv[2]) == -1) {
                error("User does not exist");
            }
        }
        
    } else if (strcmp(cmd_argv[0], "list_users") == 0 && cmd_argc == 2) {
        if ((g = find_group(group_list, cmd_argv[1])) == NULL) {
            error("Group does not exist");
        } else {
            list_users(g);
        }
        
    } else if (strcmp(cmd_argv[0], "user_balance") == 0 && cmd_argc == 3) {
        if ((g = find_group(group_list, cmd_argv[1])) == NULL) {
            error("Group does not exist");
        } else {
            if (user_balance(g, cmd_argv[2]) == -1) {
                error("User does not exist");
            }
        }
        
    } else if (strcmp(cmd_argv[0], "under_paid") == 0 && cmd_argc == 2) {
        if ((g = find_group(group_list, cmd_argv[1])) == NULL) {
            error("Group does not exist");
        } else {
            if (under_paid(g) == -1) {
                error("User list empty");
            }
        }
        
    } else if (strcmp(cmd_argv[0], "add_xct") == 0 && cmd_argc == 4) {
        if ((g = find_group(group_list, cmd_argv[1])) == NULL) {
            error("Group does not exist");
        } else {
            char *end;
            double amount = strtod(cmd_argv[3], &end);
            if (end == cmd_argv[3]) {
                error("Incorrect number format");
            } else {
                if (add_xct(g, cmd_argv[2], amount) == -1) {
                    error("User does not exist");
                }
            }
        }
    } else if(strcmp(cmd_argv[0], "recent_xct") == 0 && cmd_argc == 3) {
        if ((g = find_group(group_list, cmd_argv[1])) == NULL) {
            error("Group does not exist");
        } else {
            char *end;
            long num = strtol(cmd_argv[2], &end, 10);
            if (end == cmd_argv[2]) {
                error("Incorrect number format");
            } else {
                recent_xct(g, num);
            }
        }

    } else {
        error("Incorrect syntax");
    }
    return 0;
}

int main(int argc, char* argv[]) {
    char input[INPUT_BUFFER_SIZE];
    char *cmd_argv[INPUT_ARG_MAX_NUM];
    int cmd_argc;
    FILE *input_stream;

    /* Initialize the list head */
    Group *group_list = NULL;

    /* Batch mode */
    if (argc == 2) {
        input_stream = fopen(argv[1], "r");
        if (input_stream == NULL) {
            error("Error opening file");
            exit(1);
        }
    }
    /* Interactive mode */
    else {
        input_stream = stdin;
    }

    printf("Welcome to Buxfer!\nPlease input command:\n>");
    
    while (fgets(input, INPUT_BUFFER_SIZE, input_stream) != NULL) {
        /* Echo line if in batch mode */
        if (argc == 2) {
            printf("%s", input);
        }
        /* Tokenize arguments */
        char *next_token = strtok(input, DELIM);
        cmd_argc = 0;
        while (next_token != NULL) {
            if (cmd_argc >= INPUT_ARG_MAX_NUM - 1) {
                error("Too many arguments!");
                cmd_argc = 0;
                break;
            }
            cmd_argv[cmd_argc] = next_token;
            cmd_argc++;
            next_token = strtok(NULL, DELIM);
        }
        cmd_argv[cmd_argc] = NULL;
        if (cmd_argc > 0 && process_args(cmd_argc, cmd_argv, &group_list) == -1) {
            break; /* quit command was entered */
        }
        printf(">");
    }

    /* Close file if in batch mode */
    if (argc == 2) {
        fclose(input_stream);
    }
    return 0;
}
