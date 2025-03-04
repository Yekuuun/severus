/**
 * Author : Yekuuun
 * Github : https://github.com/Yekuuun
 * 
 * Notes : Ls base implementation => link to man : https://man7.org/linux/man-pages/man1/ls.1.html
 */

#include "cmd.h"

#define LS_OPTIONS     10
#define DEFAULT_PATH   "*"

static PARGSHANDLER options[LS_OPTIONS] = { NULL };
static WIN32_FIND_DATA findData;
static HANDLE hFind = NULL;

/**
 * Show helper commands.
 */
static VOID ShowHelper(){
    printf("LS COMMAND :\n");
    printf("\t List directory content.\n");

    printf("\n");
    printf("SYNOPSIS :\n");
    printf("\t ls [OPTION]... [PATH]...\n");

    printf("DESCRIPTION : \n");
    printf("\t List information about the files (the current dir by default).\n");

    printf("\n");
    printf("\t -a, --all\n");
    printf("\t\t do not ignore entries starting with.\n");

    printf("\n");
    printf("\t -l, --list\n");
    printf("\t\t use a long listing format.\n");

    return;
}

/**
 * Print file informations.
 */
static VOID PrintFileInfo(WIN32_FIND_DATA file, BOOL detailed) {
    if (detailed) {
        FILETIME ft;
        SYSTEMTIME st;
        FileTimeToLocalFileTime(&file.ftLastWriteTime, &ft);
        FileTimeToSystemTime(&ft, &st);

        SIZE_T sLen = strlen(file.cFileName);
        CHAR *cName = (CHAR*)malloc(sLen + 2);

        if (!cName) {
            fprintf(stderr, "Erreur : Échec d'allocation mémoire\n");
            return;
        }

        strcpy(cName, file.cFileName);

        if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            strcat(cName, "/");
        }

        printf("%02d-%02d-%04d %02d:%02d  %10lld  %s\n",
            st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute,
            ((long long)file.nFileSizeHigh << 32) | file.nFileSizeLow,
            cName);

        free(cName);
    } 
    else {
        SIZE_T sLen = strlen(findData.cFileName);
        CHAR *cName = (CHAR*)malloc(sLen + 2);
        
        if(!cName)
            return;

        strcpy(cName, findData.cFileName);
        
        if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            strcat(cName, "/");

        printf("%s  ", cName);
        free(cName);
    }
}

/**
 * Show all informations.
 */
static VOID ShowAOption(IN CHAR** args){
    while (FindNextFile(hFind, &findData)) {
        PrintFileInfo(findData, FALSE);
    }
}

/**
 * Show all informations.
 */
static VOID ShowAll(IN CHAR** args){
    while (FindNextFile(hFind, &findData)) {
        PrintFileInfo(findData, TRUE);
    }
}

/**
 * Base ls command => no args.
 */
static VOID LsBase(){
   do {
       SIZE_T sLen = strlen(findData.cFileName);
       CHAR *cName = (CHAR*)malloc(sLen + 2);
       
       if(!cName)
           break;

       strcpy(cName, findData.cFileName);
       
       if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
           strcat(cName, "/");

       printf("%s  ", cName);
       free(cName);
       
   } while (FindNextFileA(hFind, &findData) != 0);
}

/**
 * Initialize LS logic.
 */
static VOID InitializeArgs(IN PARGSHANDLER *pArgs){
    AddArg("--help", ShowHelper, pArgs);
    AddArg("-h", ShowHelper, pArgs);

    AddArg("--list", ShowAll, pArgs);
    AddArg("-l", ShowAll, pArgs);

    AddArg("--all", ShowAOption, pArgs);
    AddArg("-a", ShowAOption, pArgs);
}

/**
 * Parse LS arguments.
 */
static CHAR **ParseArguments(IN INT sLen, IN CHAR **args){
    CHAR **result = (CHAR**)malloc(2 * sizeof(CHAR*));
    result[0] = NULL;
    result[1] = strdup(DEFAULT_PATH);

    for(DWORD i = 1; i < sLen; i++){
        if(IsOption(args[i])){
            result[0] = args[i];
        }
        
        if(IsPath(args[i])){
            result[1] = args[i];
        }
    }

    return result;
}

/**
 * LS ENTRY POINT.
 */
int main(int argc, char **argv){
    if(argc > 3){
        printf("[!] Error calling ls command => consult man for help.\n");
        return EXIT_FAILURE;
    }

    CHAR **parsedArgs = ParseArguments(argc, argv);

    //initializing arguments.
    InitializeArgs(options);

    //global vars.
    CHAR cAbsolutePath[MAX_PATH] = {0};
    CHAR searchPath[MAX_PATH] = "*";

    if(*parsedArgs[1] != '*'){
        //resolve full pathname.
        if (GetFullPathNameA(parsedArgs[1], MAX_PATH, cAbsolutePath, NULL) == 0) {
            printf("$ an error occured.\n");
            return EXIT_FAILURE;
        }

        sprintf(searchPath, "%s\\*", cAbsolutePath);
    }

    hFind = FindFirstFileA(searchPath, &findData);
    if(hFind == NULL)
        return EXIT_FAILURE;

    //handling option.
    if(parsedArgs[0] == NULL){
        LsBase();
    }
    else {
        ExecOption(parsedArgs, options);
    }

__END:
    if(hFind)
        FindClose(hFind);

    return EXIT_SUCCESS;
}