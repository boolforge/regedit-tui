# Build Instructions

## Prerequisites

- CMake 3.14 or higher
- C++17 compatible compiler
- Git

## Windows Build

1. Clone the repository:
```
git clone https://github.com/boolforge/regedit-tui.git
cd regedit-tui
```

2. Create a build directory and run CMake:
```
mkdir build
cd build
cmake ..
```

3. Build the project:
```
cmake --build .
```

4. Run the application:
```
.\Debug\regedit-tui.exe
```

## Linux/macOS Build (Stub Mode)

On non-Windows platforms, the application will run in stub mode with simulated registry data.

1. Clone the repository:
```
git clone https://github.com/boolforge/regedit-tui.git
cd regedit-tui
```

2. Create a build directory and run CMake:
```
mkdir build
cd build
cmake ..
```

3. Build the project:
```
cmake --build .
```

4. Run the application:
```
./regedit-tui
```

## PowerShell Integration

To run the application from PowerShell:

1. Add the application directory to your PATH or create a PowerShell function:

```powershell
function regedit-tui {
    & "C:\path\to\regedit-tui.exe" $args
}
```

2. Add this function to your PowerShell profile to make it available in all sessions.
