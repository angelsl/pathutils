// adapted from https://support.microsoft.com/en-us/help/165721/how-to-ejecting-removable-media-in-windows-nt-windows-2000-windows-xp
// compile: cl /Ox ejectdrive.c /link User32.lib /opt:ref,icf
#define WIN32_LEAN_AND_MEAN
#define UNICODE
#define _UNICODE
#include <Windows.h>
#include <WinIoCtl.h>
#include <Cfgmgr32.h>

BOOL ioctl_noargs(HANDLE vol, DWORD ioctl) {
    DWORD bytes;
    return DeviceIoControl(vol, ioctl, NULL, 0, NULL, 0, &bytes, NULL);
}

BOOL ioctl_arg(HANDLE vol, DWORD ioctl, void *inarg, DWORD inargsz) {
    DWORD bytes;
    return DeviceIoControl(vol, ioctl, inarg, inargsz, NULL, 0, &bytes, NULL);
}

BOOL lock_volume(HANDLE vol) {
   for (int i = 0; i < 20; i++) {
        if (ioctl_noargs(vol, FSCTL_LOCK_VOLUME)) {
            return TRUE;
        }
        Sleep(500);
   }
   return FALSE;
}

enum eject_result {
    FAILED,
    DISMOUNTED,
    EJECTED
};

enum eject_result eject_volume(wchar_t drive) {
    wchar_t volName[8];
    wchar_t rootName[5];
    wsprintf(volName, L"\\\\.\\%c:", drive);
    wsprintf(rootName, L"%c:\\", drive);

    DWORD accessFlags;
    switch(GetDriveType(rootName)) {
    case DRIVE_REMOVABLE:
        accessFlags = GENERIC_READ | GENERIC_WRITE;
        break;
    case DRIVE_CDROM:
        accessFlags = GENERIC_READ;
        break;
    default:
        return FAILED;
    }

    HANDLE vol = CreateFile(volName, accessFlags,
                            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                            OPEN_EXISTING, 0, NULL);
    if (vol == INVALID_HANDLE_VALUE) {
        return FAILED;
    }

    enum eject_result result;
    if (lock_volume(vol) && ioctl_noargs(vol, FSCTL_DISMOUNT_VOLUME)) {
        result = DISMOUNTED;

        PREVENT_MEDIA_REMOVAL pmr = {
            .PreventMediaRemoval = FALSE
        };

        if (ioctl_arg(vol, IOCTL_STORAGE_MEDIA_REMOVAL, &pmr, sizeof(PREVENT_MEDIA_REMOVAL))
            && ioctl_noargs(vol, IOCTL_STORAGE_EJECT_MEDIA)) {
            result = EJECTED;
        }
    }

    // Close the volume so other processes can use the drive.
    if (!CloseHandle(vol)) {
        return FAILED;
    }

    return result;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <drive letter>\n", argv[0]);
        return 1;
    }

    enum eject_result result = eject_volume(argv[1][0]);
    switch (result) {
    case FAILED:
        printf("Failed.\n");
        return 1;
    case DISMOUNTED:
        printf("Dismounted.\n");
        return 0;
    case EJECTED:
        printf("Ejected.\n");
        return 0;
    default:
        printf("Unknown result %d.\n", result);
        return 1;
    }
}