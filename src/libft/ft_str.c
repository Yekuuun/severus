#include "libft.h"

/**
 * Get size of CHAR** array.
 * @param tab => in str array.
 */
DWORD Ft_CharArraySize(IN CHAR **tab){
    DWORD i = 0;
    while(tab[i] != NULL){
        i++;
    }

    return i;
}

/**
 * Custom implementation of strndup function.
 * @param s => in str 
 * @param n => sizeof
 */
CHAR *Ft_Strndup(CHAR *s, SIZE_T n){
    if (!s) 
        return NULL;

    SIZE_T len = strnlen(s, n);
    CHAR *cpy = (CHAR *)malloc(sizeof(CHAR) * (len + 1));
    
    if (!cpy) 
        return NULL;

    memcpy(cpy, s, len);
    cpy[len] = '\0';

    return cpy;
}