// compile: cl /Zl /Ox beep.c /link User32.lib Kernel32.lib /opt:ref,icf /entry:main /SUBSYSTEM:CONSOLE
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

void main() {
    MessageBeep(MB_OK);
    ExitProcess(0);
}
