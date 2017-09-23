// compile: cl /Zl /Ox cmdw.c /link Kernel32.lib /opt:ref,icf /entry:main /SUBSYSTEM:WINDOWS
#define WIN32_LEAN_AND_MEAN
#define UNICODE
#include <Windows.h>

void main() {
    STARTUPINFO sinfo = {
        .cb = sizeof(sinfo),
        .dwFlags = STARTF_USESHOWWINDOW,
        .wShowWindow = SW_HIDE,
    };
    PROCESS_INFORMATION pinfo;

    if (!CreateProcess(L"C:\\Windows\\System32\\cmd.exe", GetCommandLineW(),
                       NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL,
                       &sinfo, &pinfo)) {
        ExitProcess(GetLastError());
    }

    ExitProcess(0);
}
