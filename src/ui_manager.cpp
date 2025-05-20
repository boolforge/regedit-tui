#include "ui_manager.h"
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <iostream>

namespace ui {

UIManager::UIManager() 
    : registry_manager_(registry::RegistryManager::Create()),
      current_path_("HKEY_LOCAL_MACHINE\\SOFTWARE"),
      current_view_(View::Keys),
      screen_(ftxui::ScreenInteractive::Fullscreen()) {
    InitializeUI();
}

void UIManager::InitializeUI() {
    main_container_ = CreateMainLayout();
}

void UIManager::Run() {
    screen_.Loop(main_container_);
}

ftxui::Component UIManager::CreateMainLayout() {
    auto navigation_panel = CreateNavigationPanel();
    auto content_panel = CreateContentPanel();
    auto status_bar = CreateStatusBar();
    auto help_bar = CreateHelpBar();
    
    auto layout = ftxui::Container::Vertical({
        ftxui::Container::Horizontal({
            navigation_panel,
            content_panel
        }),
        status_bar,
        help_bar
    });
    
    return layout;
}

ftxui::Component UIManager::CreateNavigationPanel() {
    // Get subkeys for the current path
    auto subkeys = registry_manager_->GetSubkeys(current_path_);
    
    // Create a list of menu entries
    std::vector<std::string> entries;
    entries.push_back("..");  // Parent directory
    for (const auto& subkey : subkeys) {
        entries.push_back(subkey);
    }
    
    // Create a menu component
    int selected = 0;
    auto menu = ftxui::Menu(&entries, &selected);
    
    // Add a border and a title
    menu = ftxui::Renderer(menu, [&] {
        return ftxui::window(
            ftxui::text("Registry Keys") | ftxui::bold,
            menu->Render() | ftxui::frame
        );
    });
    
    // Add event handler for navigation
    menu |= ftxui::CatchEvent([&](ftxui::Event event) {
        if (event == ftxui::Event::Return) {
            if (selected == 0) {
                // Navigate to parent
                NavigateToParent();
            } else {
                // Navigate to child
                NavigateToChild(entries[selected]);
            }
            return true;
        }
        return false;
    });
    
    return menu;
}

ftxui::Component UIManager::CreateContentPanel() {
    // Get values for the current path
    auto values = registry_manager_->GetValues(current_path_);
    
    // Create a table with name, type, and data columns
    std::vector<std::string> names;
    std::vector<std::string> types;
    std::vector<std::string> data_values;
    
    for (const auto& value : values) {
        names.push_back(value.name);
        types.push_back(registry::RegistryManager::ValueTypeToString(value.type));
        data_values.push_back(registry::RegistryManager::ValueDataToString(value));
    }
    
    // Create a menu component for the values
    int selected = 0;
    auto menu = ftxui::Menu(&names, &selected);
    
    // Add a border and a title
    menu = ftxui::Renderer(menu, [&](ftxui::Element inner) {
        std::vector<ftxui::Element> table;
        
        // Header row
        table.push_back(
            ftxui::hbox({
                ftxui::text("Name") | ftxui::bold | ftxui::flex,
                ftxui::text("Type") | ftxui::bold | ftxui::width(15),
                ftxui::text("Data") | ftxui::bold | ftxui::flex
            })
        );
        
        // Separator
        table.push_back(ftxui::separator());
        
        // Data rows
        for (size_t i = 0; i < names.size(); ++i) {
            auto style = i == static_cast<size_t>(selected) ? ftxui::inverted : ftxui::nothing;
            table.push_back(
                ftxui::hbox({
                    ftxui::text(names[i]) | style | ftxui::flex,
                    ftxui::text(types[i]) | style | ftxui::width(15),
                    ftxui::text(data_values[i]) | style | ftxui::flex
                })
            );
        }
        
        return ftxui::window(
            ftxui::text("Registry Values") | ftxui::bold,
            ftxui::vbox(table) | ftxui::frame
        );
    });
    
    // Add event handler for editing values
    menu |= ftxui::CatchEvent([&](ftxui::Event event) {
        if (event == ftxui::Event::Return && !names.empty()) {
            selected_value_ = names[selected];
            EditSelectedValue();
            return true;
        }
        return false;
    });
    
    return menu;
}

ftxui::Component UIManager::CreateStatusBar() {
    return ftxui::Renderer([&] {
        return ftxui::hbox({
            ftxui::text("Path: ") | ftxui::bold,
            ftxui::text(current_path_)
        }) | ftxui::border;
    });
}

ftxui::Component UIManager::CreateHelpBar() {
    return ftxui::Renderer([&] {
        return ftxui::hbox({
            ftxui::text("F1:Help") | ftxui::bold,
            ftxui::text(" | "),
            ftxui::text("F2:New Key") | ftxui::bold,
            ftxui::text(" | "),
            ftxui::text("F3:New Value") | ftxui::bold,
            ftxui::text(" | "),
            ftxui::text("Del:Delete") | ftxui::bold,
            ftxui::text(" | "),
            ftxui::text("F5:Refresh") | ftxui::bold,
            ftxui::text(" | "),
            ftxui::text("F10:Exit") | ftxui::bold
        }) | ftxui::border;
    });
}

void UIManager::NavigateToParent() {
    size_t pos = current_path_.find_last_of('\\');
    if (pos != std::string::npos) {
        current_path_ = current_path_.substr(0, pos);
        RefreshCurrentView();
    }
}

void UIManager::NavigateToChild(const std::string& child) {
    if (child != "..") {
        current_path_ += "\\" + child;
        RefreshCurrentView();
    }
}

void UIManager::RefreshCurrentView() {
    // This would rebuild the UI components
    // For now, we'll just print to console for debugging
    std::cout << "Refreshing view for path: " << current_path_ << std::endl;
}

void UIManager::CreateNewKey() {
    // This would show a dialog to create a new key
    // For now, we'll just print to console for debugging
    std::cout << "Creating new key in: " << current_path_ << std::endl;
}

void UIManager::DeleteSelectedKey() {
    // This would show a confirmation dialog and delete the key
    // For now, we'll just print to console for debugging
    std::cout << "Deleting key: " << selected_key_ << std::endl;
}

void UIManager::CreateNewValue() {
    // This would show a dialog to create a new value
    // For now, we'll just print to console for debugging
    std::cout << "Creating new value in: " << current_path_ << std::endl;
}

void UIManager::EditSelectedValue() {
    // This would show a dialog to edit the selected value
    // For now, we'll just print to console for debugging
    std::cout << "Editing value: " << selected_value_ << std::endl;
}

void UIManager::DeleteSelectedValue() {
    // This would show a confirmation dialog and delete the value
    // For now, we'll just print to console for debugging
    std::cout << "Deleting value: " << selected_value_ << std::endl;
}

void UIManager::ImportRegistry() {
    // This would show a dialog to import registry data
    // For now, we'll just print to console for debugging
    std::cout << "Importing registry data" << std::endl;
}

void UIManager::ExportRegistry() {
    // This would show a dialog to export registry data
    // For now, we'll just print to console for debugging
    std::cout << "Exporting registry data from: " << current_path_ << std::endl;
}

void UIManager::SearchRegistry() {
    // This would show a dialog to search the registry
    // For now, we'll just print to console for debugging
    std::cout << "Searching registry" << std::endl;
}

} // namespace ui
