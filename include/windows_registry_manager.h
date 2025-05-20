#pragma once

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#include <string>
#include <vector>
#include <memory>
#include "registry_manager.h"

namespace registry {

// Windows-specific implementation of RegistryManager
class WindowsRegistryManager : public RegistryManager {
public:
    WindowsRegistryManager();
    ~WindowsRegistryManager();

    std::optional<Key> OpenKey(const std::string& path) override;
    std::vector<Value> GetValues(const std::string& path) override;
    std::vector<std::string> GetSubkeys(const std::string& path) override;
    bool CreateKey(const std::string& path) override;
    bool DeleteKey(const std::string& path) override;
    bool SetValue(const std::string& path, const Value& value) override;
    bool DeleteValue(const std::string& path, const std::string& valueName) override;

private:
    // Helper methods
    HKEY GetRootKeyHandle(const std::string& rootKeyName);
    std::pair<HKEY, std::string> ParseRegistryPath(const std::string& path);
    ValueType GetValueType(DWORD winType);
    DWORD GetWinType(ValueType type);
    ValueData ReadValueData(HKEY hKey, const std::string& valueName, ValueType type);
};

} // namespace registry
#endif
