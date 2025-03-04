/**
 * Author : Yekuuun
 * Github : https://github.com/Yekuuun
 * 
 * Notes  : My implementation of tree command.
 */

 #include "cmd.h"

/**
 * Setting up console for display.
 */
VOID SetupConsole() {
    SetConsoleOutputCP(CP_UTF8);

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    DWORD mode = 0;
    GetConsoleMode(hConsole, &mode);
    SetConsoleMode(hConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

/**
 * Printing prefixes properly.
 * @param ctx => ThreeContext ptr
 * @param level => current level
 */
VOID PrintPrefix(PTreeContext ctx, int level) {
    for (int i = 0; i < level; i++) {
        if (i == level - 1) {
            printf(ctx->isLastChild[i] ? "└───" : "├───");
        } 
        else {
            printf(ctx->isLastChild[i] ? "    " : "│   ");
        }
    }
}

/**
 * Main TREE function
 * @param path => path for search (dir)
 * @param level => current level
 * @param ctx => ThreeContext
 */
VOID Tree(IN const char* path, IN INT level, IN PTreeContext ctx){
    WIN32_FIND_DATA findData; 
    HANDLE hFind = NULL;

    CHAR searchPath[MAX_TREE_DEPTH];
    CHAR nextPath[MAX_TREE_DEPTH];

    sprintf(searchPath, "%s\\*", path); //building search path.

    hFind = FindFirstFile(searchPath, &findData);
    if(hFind == INVALID_HANDLE_VALUE)
        return;
    
    int count = 0;

    while(FindNextFile(hFind, &findData)){
        if(strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0)
            continue;
        
        count++;
    }

    FindClose(hFind);
    hFind = FindFirstFile(searchPath, &findData);

    if(hFind == INVALID_HANDLE_VALUE)
        return;
    
    INT currentItem = 0;

    while(FindNextFile(hFind, &findData)){
        if(strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0)
            continue;
        
        currentItem++;
        ctx->isLastChild[level] = (currentItem == count);

        PrintPrefix(ctx, level);
        printf("%s\n", findData.cFileName);

        if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
            sprintf(nextPath, "%s\\%s", path, findData.cFileName);
            ctx->dirCount++;
            Tree(nextPath, level + 1, ctx);
        }
        else {
            ctx->fileCount++;
        }
    }

    FindClose(hFind);
}

//entry point.
int main(int argc, char *argv[]){
    if(argc > 2){
        printf("$ tree accept only one argument : <path_to_folder>\n");
        return EXIT_FAILURE;
    }

    CHAR* path = NULL;

    if(argc == 2) {
        const CHAR* arg = argv[1];
        
        //check validity.
        if(!IsValidPath(arg))
            return EXIT_FAILURE;

        INT argLen = strlen(arg);
        path = malloc((argLen + 1) * sizeof(CHAR));

        if(path == NULL){
            printf("[!] Error allocating memory.\n");
            return EXIT_FAILURE;
        }
        
        memcpy(path, arg, argLen);
        path[argLen] = '\0';
    }
    else {
        path = malloc(MAX_TREE_DEPTH * sizeof(char));
        if(path == NULL)
            return EXIT_FAILURE;

        path = strdup(".");

        DWORD len = GetCurrentDirectory(MAX_TREE_DEPTH, path);
        if(len == 0)
            return EXIT_FAILURE;
    }

    SetupConsole();
    TreeContext ctx = {0};
    RtlSecureZeroMemory(&ctx, sizeof(TreeContext));

    printf("%s\n", path);
    Tree(path, 0, &ctx);

    printf("\n");
    printf("%d directories, %d files\n", ctx.dirCount, ctx.fileCount);

__END:
    if(path)
        free(path);
    
    return EXIT_SUCCESS;
}