// compile: cl /Zl /Ox startw.c /link Kernel32.lib Shell32.lib /opt:ref,icf /entry:main /SUBSYSTEM:WINDOWS
#define WIN32_LEAN_AND_MEAN
#define UNICODE
#include <Windows.h>
#include <Shellapi.h>

void main() {
    STARTUPINFO sinfo = {
        .cb = sizeof(sinfo),
        .dwFlags = STARTF_USESHOWWINDOW,
        .wShowWindow = SW_HIDE,
    };
    PROCESS_INFORMATION pinfo;

    int inquote = 0;
    int backslash = 0;
    LPWSTR rawarg = GetCommandLineW();
    for (;;) {
        int exit = 0;
        switch (*rawarg) {
            case L'\\':
                backslash = 2;
                break;
            case L'"':
                if (!backslash) {
                    inquote = !inquote;
                }
                break;
            case L' ':
                if (!inquote) {
                    exit = 1;
                }
                break;
        }
        if (exit) {
            break;
        }
        if (backslash > 0) {
            --backslash;
        }
        rawarg++;
    }
    
    while (*rawarg == L' ') {
        rawarg++;
    }
    
    if (!CreateProcess(NULL, rawarg,
                       NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL,
                       &sinfo, &pinfo)) {
        ExitProcess(GetLastError());
    }

    ExitProcess(0);
}
