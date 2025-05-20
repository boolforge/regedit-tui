#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <memory>
#include <string>
#include <vector>

#include "registry_manager.h"

namespace ui {

// UI Manager class to handle the terminal user interface
class UIManager {
public:
    UIManager();
    ~UIManager() = default;

    // Run the UI
    void Run();

private:
    // Registry manager
    std::unique_ptr<registry::RegistryManager> registry_manager_;

    // Current path in registry
    std::string current_path_;

    // Current selected key
    std::string selected_key_;

    // Current selected value
    std::string selected_value_;

    // Current view (keys, values)
    enum class View { Keys, Values };
    View current_view_;

    // UI components
    ftxui::Component main_container_;
    ftxui::ScreenInteractive screen_;

    // Initialize UI components
    void InitializeUI();

    // Create the main layout
    ftxui::Component CreateMainLayout();

    // Create the navigation panel
    ftxui::Component CreateNavigationPanel();

    // Create the content panel
    ftxui::Component CreateContentPanel();

    // Create the status bar
    ftxui::Component CreateStatusBar();

    // Create the help bar
    ftxui::Component CreateHelpBar();

    // Navigation handlers
    void NavigateToParent();
    void NavigateToChild(const std::string& child);
    void RefreshCurrentView();

    // Action handlers
    void CreateNewKey();
    void DeleteSelectedKey();
    void CreateNewValue();
    void EditSelectedValue();
    void DeleteSelectedValue();
    void ImportRegistry();
    void ExportRegistry();
    void SearchRegistry();
};

} // namespace ui
