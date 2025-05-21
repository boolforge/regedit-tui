# User Guide

## Overview

regedit-tui is a terminal-based registry editor for Windows that can run under PowerShell. It provides a text-based interface for navigating and manipulating the Windows Registry, similar to how ncurses-based tools work in Unix environments.

## Features

- Terminal-based navigation of the Windows Registry
- View, edit, create, and delete registry keys and values
- Support for all registry value types
- Cross-platform UI (with stub implementation on non-Windows platforms)
- PowerShell integration

## Usage

### Basic Navigation

- Use arrow keys to navigate through registry keys and values
- Press Enter to select a key or edit a value
- Press Backspace or select ".." to navigate to the parent key

### Keyboard Shortcuts

- F1: Show help
- F2: Create a new registry key
- F3: Create a new registry value
- Del: Delete the selected key or value
- F5: Refresh the current view
- F10: Exit the application

### Editing Values

When editing registry values:
1. Select the value you want to edit
2. Press Enter to open the edit dialog
3. Modify the value according to its type
4. Press Enter to save or Esc to cancel

## PowerShell Integration

To run regedit-tui from PowerShell, you can:

1. Navigate to the directory containing the executable and run:
```powershell
.\regedit-tui.exe
```

2. Add the application directory to your PATH or create a PowerShell function:
```powershell
function regedit-tui {
    & "C:\path\to\regedit-tui.exe" $args
}
```

## Limitations

- On non-Windows platforms, the application runs in stub mode with simulated registry data
- Some advanced registry operations may not be supported in the current version
