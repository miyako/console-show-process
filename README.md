# console-show-process

```
Usage: show-process -p process {-s option}
```

Show every non-owned window of a process.

```
 -p process: process id
 -s option: what to do with the window (default=SW_SHOWNORMAL:1)
 SW_HIDE:0
 SW_SHOWNORMAL:1
 SW_SHOWMINIMIZED:2
 SW_SHOWMAXIMIZED:3
 SW_SHOWNOACTIVATE:4
 SW_SHOW:5
 SW_MINIMIZE:6
 SW_SHOWMINNOACTIVE:7
 SW_SHOWNA:8
 SW_RESTORE:9
 SW_SHOWDEFAULT:10
 SW_FORCEMINIMIZE:11
```

### What qualifies as a Main Window?

[EnumWindows](https://msdn.microsoft.com/en-us/library/windows/desktop/ms633497(v=vs.85).aspx) is used to enumerate all windows.

[GetWindowThreadProcessId](https://msdn.microsoft.com/en-us/library/windows/desktop/ms633522(v=vs.85).aspx) is used to examine the process ID and thread ID of each window.

If PID matches, [GetWindow](https://msdn.microsoft.com/en-us/library/windows/desktop/ms633515(v=vs.85).aspx) is used to filter any [owned windows](https://msdn.microsoft.com/en-us/library/windows/desktop/ms632599(v=vs.85).aspx#owned_windows).

If window is not owned, [GetGUIThreadInfo](https://msdn.microsoft.com/en-us/library/windows/desktop/ms633506(v=vs.85).aspx) is used to filter non-active-GUI windows.

The assumption here is that what we call a main window must not be owned and must be an active GUI window.
