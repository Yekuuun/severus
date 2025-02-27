#include "root.h"

static volatile int KEEP_RUNNING = 1;

/**
 * Handling CTRL-C events.
 */
static BOOL WINAPI HandleCtrlC(IN DWORD dwType){
    return dwType == CTRL_C_EVENT;
}

/**
 * Displaying current DIR.
 */
static VOID DisplayCurrentDir(){
    LPSTR lpPath[MAX_PATH] = {0};

    DWORD dwPath = GetCurrentDirectoryA(MAX_PATH, lpPath);
    if(dwPath != 0)
        printf("%s\n", lpPath);
}

/**
 * Display current username.
 */
static VOID DisplayUsername(){
    LPSTR lpUsername[256] = {0};
    DWORD dwUsername = sizeof(lpUsername);

    if(GetUserNameA(lpUsername, &dwUsername))
        printf("%s~", lpUsername);
}

/**
 * Display shell header.
 */
static VOID DisplayShellHeader(){
    DisplayUsername();
    DisplayCurrentDir();
}

/**
 * Clear console & set cursor to begin of the window.
 */
static VOID ClearConsole() {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == NULL)
        return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hStdOut, &dwMode))
        return;

    const DWORD dwOriginalMode = dwMode;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    if (!SetConsoleMode(hStdOut, dwMode))
        return;

    DWORD dwWritten = 0;
    PCWSTR pwSeq = L"\x1b[2J\x1b[H";  // Efface et remet le curseur en haut

    if (!WriteConsoleW(hStdOut, pwSeq, (DWORD)wcslen(pwSeq), &dwWritten, NULL)) {
        SetConsoleMode(hStdOut, dwOriginalMode);
        return;
    }

    SetConsoleMode(hStdOut, dwOriginalMode);
}


/**
 * Start shell program.
 */
VOID RunShell(){
    ClearConsole();
    CHAR cBuffer[MAX_TOKENS_LEN] = {0};
    
    //setting up listener.
    if(!SetConsoleCtrlHandler(HandleCtrlC, TRUE)){
        printf("[!] Error setting up event listener. \n");
        exit(0);
    }

    //looping.
    while(KEEP_RUNNING){
        DisplayShellHeader();
        printf("$ ");

        if(fgets(cBuffer, sizeof(cBuffer), stdin) != NULL){
            printf("%s", cBuffer);
            Sleep(1);
        }
        else {
            printf("[$] ENDING severus...");
            KEEP_RUNNING = 0;
        }
    }
    
    return;
}