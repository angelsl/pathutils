Some small utilities I put in my path.

### windows/beep.c

Beeps. I use it at the end of a long-running command:

`make ... & beep`

### windows/admincmd.c

Launches `cmd.exe` elevated. I use it with an Explorer context menu entry for a quick open-admin-prompt-here shortcut since they removed the shortcut in Explorer's menu.

See `windows/bgcmd_user.reg`.

### windows/cmdw.c

Lets you launch `cmd.exe` without a command window.

I use it in a shortcut to launch VS Code with a modified `PATH`, since VS Code only works with `node` in the PATH and I don't keep it in my default `PATH`.

`cmdw /c apath nodejs & start "" /D "C:\Program Files\Microsoft VS Code\" /B "C:\Program Files\Microsoft VS Code\Code.exe"`

### windows/bgcmd.reg

Removes the `Extended` key from the _Open command window here_/_Open PowerShell window here_ context menu entries, so it shows up without holding Shift.

Also removes the `HideBasedOnVelocityId` key, so _Open command window here_ will show up in newer versions of Windows 10.

Needs to be [run as TrustedInstaller](https://wj32.org/processhacker/forums/viewtopic.php?f=18&t=2407).

### windows/bgcmd_user.reg

Adds _Open MSYS2 here_ and _Open admin prompt_ to the context menu.

Needs to be run as the current user. If you want it to be system-wide, change all `HKCU` to `HKLM`.