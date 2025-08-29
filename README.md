# Keylogger for Windows
## Overview
This project is a Windows-based keylogger developed in C using Windows APIs, designed for auditing purposes. It captures keystrokes and mouse events, logs them with timestamps, and optionally sends logs to a remote server. The keylogger operates in stealth mode and is intended for use in controlled environments with explicit permission.

## Features

1. Captures all keystrokes (letters, numbers, symbols, special keys like Shift, Ctrl, Enter).
2. Tracks mouse events (left and right clicks).
3. Logs events with millisecond-precision timestamps.
4. Runs in stealth mode by hiding the console window.
5. Optionally sends logs to a remote Flask server via HTTP POST.

## Requirements

**OS:** Windows 11

**Compiler:** GCC (MinGW)

**Libraries:** user32.dll (linked with -luser32)

**Dependencies:** Windows API functions (SetWindowsHookEx, GetAsyncKeyState, ShowWindow)

## Setup Instructions

1. Install MinGW: Download and install MinGW to compile C code on Windows.

2. Clone or download the keylogger.c file.

3. Ensure user32.dll is accessible (included in Windows by default).

4. Compile the program:
```
gcc -o keylogger.exe keylogger.c -luser32

```


Resolve errors like "No such file or directory" by checking file paths and the working directory.

5. (Optional) Set up a Flask server for log transmission (see log_server_readme.md).


## Usage

1. Run the compiled executable:

```
./keylogger.exe
```


2. The keylogger runs silently (console hidden) and logs events to a local file (e.g., keylog.txt).

3. Logs include timestamps and decoded keystrokes/mouse events.

4. To send logs to a server, configure the server URL in the code.

5. Stop the program by terminating the process (e.g., via Task Manager).


## Considerations

Likely flagged by antivirus software.

## Challenges Overcome

1. Fixed compilation errors (e.g., file path issues).

2. Resolved user32.dll linking issues with -luser32.

3. Handled raw log decoding for meaningful output.

4. Eliminated duplicate keystrokes from press/release events.

5. Managed case sensitivity and special key logging.

6. Improved mouse click accuracy.

7. Implemented stealth mode with ShowWindow(SW_HIDE).

## License
This project is for educational purposes only and is not licensed for commercial or unethical use.
