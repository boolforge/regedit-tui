#include <iostream>
#include "ui_manager.h"

int main(int argc, char* argv[]) {
    std::cout << "Starting regedit-tui..." << std::endl;
    
    try {
        ui::UIManager ui_manager;
        ui_manager.Run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
        return 1;
    }
    
    return 0;
}
