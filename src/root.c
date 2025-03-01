/**
 * Author : yekuuun
 * Github : https://github.com/yekuuun
 * 
 * Notes  : Entry point of severus.
 */

#include "root.h"

static volatile int KEEP_RUNNING = 1;

/**
 * Handling CTRL-C events.
 */
static BOOL WINAPI HandleCtrlC(IN DWORD dwType){
    return dwType == CTRL_C_EVENT;
}

/**
 * Display current username.
 */
static VOID DisplayUsername(){
    LPSTR lpUsername[256] = {0};
    DWORD dwUsername = sizeof(lpUsername);

    if(GetUserNameA(lpUsername, (LPDWORD)&dwUsername))
        printf("%s~", lpUsername);
}

/**
 * Display shell header.
 */
static VOID DisplayShellHeader(){
    DisplayUsername();
    Pwd();
}


/**
 * Start shell program.
 */
VOID RunShell(){
    PCOMMANDHISTORY pHistory = InitializeHistory(MAX_HISTORY);
    if(pHistory == NULL){
        printf("[!] Error initializing SEVERUS.\n");
        return;
    }

    InitBuiltins();

    Clean();
    CHAR cBuffer[MAX_TOKENS_LEN] = {0};
    
    //setting up listener.
    if(!SetConsoleCtrlHandler(HandleCtrlC, TRUE)){
        FreeHistory(pHistory);
        exit(1);
    }

    //looping.
    while(KEEP_RUNNING){
        DisplayShellHeader();
        printf("$ ");

        if(fgets(cBuffer, sizeof(cBuffer), stdin) != NULL){
            cBuffer[strcspn(cBuffer, "\n")] = '\0';

            CHAR *args[MAX_TOKENS_LEN] = {0};
            CHAR *token = strtok(cBuffer, " ");
            if(!token) continue;

            args[0] = token;
            DWORD i = 1;
            while ((token = strtok(NULL, " ")) && i < MAX_TOKENS_LEN - 1) {
                args[i++] = token;
            }
            args[i] = NULL;

            if(isBuiltin(args[0])){
                ExecBuiltin(args);
            }
            else {
                printf("%s\n", args[0]);
            }

            //-------------------------------
            AddToHistory(pHistory, cBuffer);
            Sleep(1);
        }
        else {
            KEEP_RUNNING = 0;
        }
    }
    
    FreeHistory(pHistory);
    return;
}