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
 * Set cmd color 
 * @param color => WORD color
 */
static VOID SetBashColor(IN WORD color){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if(hConsole)
        SetConsoleTextAttribute(hConsole, color);
}

/**
 * Display shell header.
 */
static VOID DisplayShellHeader(){
    SetBashColor(10);
    DisplayUsername();
    Pwd();

    //go back to main color.
    SetBashColor(7);
}

/**
 * Start shell program.
 */
VOID RunShell(){
    PTOKEN pTokens            = NULL;
    PCOMMAND pCommands        = NULL;
    PCOMMANDHISTORY pHistory  = NULL;

    //history.
    pHistory = InitializeHistory(MAX_HISTORY);
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

            //free commands.
            if(pCommands){
                FreeCommands(pCommands);
                pCommands = NULL;
            }

            //lex.
            Lexer(cBuffer, &pTokens);

            //parse.
            pCommands = ParseTokens(pTokens);

            if(isBuiltin(pCommands->args[0])) {
                ExecBuiltin(pCommands->args); //exec builtin.
            }
            else {
                printf("%s\n", cBuffer);
            }

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