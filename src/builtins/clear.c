#include "builtin.h"

/**
 * Clearing console = BUILTIN CLS.
 */
VOID Clean() {
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