cmake_minimum_required(VERSION 3.14)
project(regedit-tui VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include FetchContent for downloading dependencies
include(FetchContent)

# Fetch FTXUI
FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v3.0.0
)
FetchContent_MakeAvailable(ftxui)

# Add executable
add_executable(regedit-tui 
  src/main.cpp
  src/registry_manager.cpp
  src/ui_manager.cpp
)

# Include directories
target_include_directories(regedit-tui PRIVATE include)

# Link libraries
target_link_libraries(regedit-tui
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)

# Platform-specific settings
if(WIN32)
  target_compile_definitions(regedit-tui PRIVATE PLATFORM_WINDOWS)
elseif(APPLE)
  target_compile_definitions(regedit-tui PRIVATE PLATFORM_MACOS)
elseif(UNIX)
  target_compile_definitions(regedit-tui PRIVATE PLATFORM_LINUX)
endif()

# Install
install(TARGETS regedit-tui DESTINATION bin)
