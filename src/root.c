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
 * @param dwType => type of signal.
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

    //tokens.
    PTOKEN pTokens = NULL;

    //history.
    PCOMMANDHISTORY pHistory = InitializeHistory(MAX_HISTORY);
    if(pHistory == NULL){
        printf("[!] Error initializing SEVERUS.\n");
        return;
    }

    //init builtins commands.
    InitBuiltins();

    //clean console.
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

            //free tokens.
            if (pTokens) {
                FreeTokens(pTokens);
                pTokens = NULL;
            }

            //lex.
            Lexer(cBuffer, &pTokens);

            //parse.
            //TO DO.

            // ExecBuiltin(args);

            //-------------------------------
            AddToHistory(pHistory, cBuffer);
            Sleep(1);
        }
        else {
            KEEP_RUNNING = 0;
        }
    }
    
    if(pTokens)
        FreeTokens(pTokens);
    
    if(pHistory)
        FreeHistory(pHistory);

    return;
}