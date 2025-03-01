#include "builtin.h"

/**
 * main exit function.
 */
VOID Exit(IN CHAR **args){
    INT EXIT_CODE = 0;

    if(args[1] != NULL)
        EXIT_CODE = atoi(args[1]);

    printf("$ Exiting severus. CIAO....\n");
    exit(EXIT_CODE);
}