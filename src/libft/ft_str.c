#include "libft.h"

/**
 * Get size of CHAR** array.
 */
DWORD GetArrayCharSize(IN CHAR **tab){
    DWORD i = 0;
    while(tab[i] != NULL){
        i++;
    }

    return i;
}