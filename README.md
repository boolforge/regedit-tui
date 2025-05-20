# regedit-tui

A terminal-based registry editor for Windows that can run under PowerShell.

## Overview

This project aims to create a text-based, ncurses-style version of the Windows Registry Editor (regedit) that can run within a PowerShell terminal. It provides a terminal user interface (TUI) for navigating and manipulating the Windows Registry, similar to how ncurses-based tools work in Unix environments.

## Features (Planned)

- Terminal-based navigation of the Windows Registry
- View, edit, create, and delete registry keys and values
- Search functionality for keys and values
- Import/export registry sections
- Cross-platform UI (Windows, Linux, macOS)
- PowerShell integration

## Technology Stack

- **UI Library**: FTXUI (Functional Terminal User Interface)
- **Registry Access**: WinReg (C++ wrapper for Windows Registry API)
- **Language**: C++
- **Build System**: CMake

## Project Status

This project is currently in early development. Research and design phases are complete, and implementation is in progress.

## License

MIT

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.
