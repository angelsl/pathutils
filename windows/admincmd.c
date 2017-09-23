/* compile: cl /GS- /Zl /Oi /Ox /Wall admincmd.c /link Shell32.lib Kernel32.lib Ole32.lib /opt:ref,icf /entry:main /SUBSYSTEM:WINDOWS */
#define WIN32_LEAN_AND_MEAN
#define UNICODE
#include <Windows.h>
#include <Objbase.h>
#include <Shellapi.h>

#pragma function(memset)
void *memset(void *pTarget, int value, size_t cbTarget) {
    char *cur = pTarget;
    for (size_t c = 0; c < cbTarget; ++c) {
        *(cur++) = (char)value;
    }
    return pTarget;
}

int main(void) {
    int coInitResult = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    SHELLEXECUTEINFOW exInfo = {
        .cbSize = sizeof(exInfo),
        .fMask = SEE_MASK_NOASYNC ,
        .lpVerb = L"runas",
        .lpFile = L"C:\\Windows\\System32\\cmd.exe",
        .nShow = SW_SHOW
    };
    WCHAR setPwd[1034] = L"/k cd /d ";
    if (GetCurrentDirectory(1024, setPwd + 9)) {
        exInfo.lpParameters = setPwd;
    }
    
    int exit = ShellExecuteExW(&exInfo) ? 0 : 1;
    if (coInitResult == S_OK || coInitResult == S_FALSE) {
        CoUninitialize();
    }
    ExitProcess(exit);
}