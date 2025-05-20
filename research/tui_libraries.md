# C++ TUI Libraries Research

## Overview
This document summarizes research on C++ Terminal User Interface (TUI) libraries that could be used for implementing a terminal-based registry editor.

## Key Libraries

### 1. ncurses
- **Description**: The classic and most established TUI library
- **Pros**:
  - Widely used and supported
  - Extensive documentation and community support
  - Available on most platforms
  - Mature and stable
- **Cons**:
  - C-based API (not modern C++)
  - Complex API with steep learning curve
  - Requires platform-specific adaptations

### 2. FTXUI (Functional Terminal User Interface)
- **Description**: Modern C++ library specifically designed for terminal interfaces
- **Pros**:
  - Modern C++ design (C++17)
  - Declarative and component-based approach
  - Cross-platform (Windows, Linux, macOS)
  - Actively maintained
  - Simpler API than ncurses
- **Cons**:
  - Newer library with smaller community
  - Less comprehensive documentation

### 3. cpp-terminal
- **Description**: Lightweight C++ library for terminal manipulation
- **Pros**:
  - Cross-platform (Windows, Linux, macOS)
  - Simple API
  - Modern C++ design
  - Small footprint
- **Cons**:
  - Limited feature set compared to ncurses or FTXUI
  - Smaller community and less documentation

## Recommendation
Based on the requirements for ease of implementation and cross-platform compatibility, **FTXUI** appears to be the best choice for this project because:

1. It offers a modern C++ API that is easier to work with than ncurses
2. It provides cross-platform support out of the box
3. It has a component-based architecture that will simplify UI development
4. It's actively maintained and growing in popularity

## References
- [FTXUI GitHub Repository](https://github.com/ArthurSonzogni/FTXUI)
- [cpp-terminal GitHub Repository](https://github.com/jupyter-xeus/cpp-terminal)
- [ncurses Official Site](https://invisible-island.net/ncurses/)
- [Reddit Discussion on C++ TUI Libraries](https://www.reddit.com/r/cpp/comments/ppyv53/is_there_a_defacto_standard_of_c_tui_library/)
