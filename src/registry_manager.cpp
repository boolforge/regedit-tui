#include "registry_manager.h"

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#include <winreg.h>
#endif

#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>

namespace registry {

// Static methods implementation
std::string RegistryManager::ValueTypeToString(ValueType type) {
    switch (type) {
        case ValueType::REG_NONE: return "REG_NONE";
        case ValueType::REG_SZ: return "REG_SZ";
        case ValueType::REG_EXPAND_SZ: return "REG_EXPAND_SZ";
        case ValueType::REG_BINARY: return "REG_BINARY";
        case ValueType::REG_DWORD: return "REG_DWORD";
        case ValueType::REG_DWORD_BIG_ENDIAN: return "REG_DWORD_BIG_ENDIAN";
        case ValueType::REG_LINK: return "REG_LINK";
        case ValueType::REG_MULTI_SZ: return "REG_MULTI_SZ";
        case ValueType::REG_RESOURCE_LIST: return "REG_RESOURCE_LIST";
        case ValueType::REG_QWORD: return "REG_QWORD";
        default: return "UNKNOWN";
    }
}

std::string RegistryManager::ValueDataToString(const Value& value) {
    std::ostringstream oss;
    
    switch (value.type) {
        case ValueType::REG_NONE:
            oss << "(none)";
            break;
        case ValueType::REG_SZ:
        case ValueType::REG_EXPAND_SZ:
            oss << std::get<std::string>(value.data);
            break;
        case ValueType::REG_BINARY: {
            const auto& data = std::get<std::vector<uint8_t>>(value.data);
            for (const auto& byte : data) {
                oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";
            }
            break;
        }
        case ValueType::REG_DWORD:
        case ValueType::REG_DWORD_BIG_ENDIAN:
            oss << "0x" << std::hex << std::get<uint32_t>(value.data) << " (" 
                << std::dec << std::get<uint32_t>(value.data) << ")";
            break;
        case ValueType::REG_QWORD:
            oss << "0x" << std::hex << std::get<uint64_t>(value.data) << " (" 
                << std::dec << std::get<uint64_t>(value.data) << ")";
            break;
        case ValueType::REG_MULTI_SZ: {
            const auto& strings = std::get<std::vector<std::string>>(value.data);
            for (size_t i = 0; i < strings.size(); ++i) {
                oss << strings[i];
                if (i < strings.size() - 1) {
                    oss << ", ";
                }
            }
            break;
        }
        default:
            oss << "(unsupported type)";
            break;
    }
    
    return oss.str();
}

// Factory method implementation
std::unique_ptr<RegistryManager> RegistryManager::Create() {
    // This would create the appropriate implementation based on platform
    // For now, we'll return a stub implementation
    class StubRegistryManager : public RegistryManager {
    public:
        std::optional<Key> OpenKey(const std::string& path) override {
            Key key;
            key.name = path.substr(path.find_last_of('\\') + 1);
            key.path = path;
            
            // Add some dummy values for testing
            key.values = {
                {"Default", ValueType::REG_SZ, std::string("(value not set)")},
                {"Version", ValueType::REG_DWORD, uint32_t(1)},
                {"Description", ValueType::REG_SZ, std::string("Registry TUI Editor")}
            };
            
            // Add some dummy subkeys for testing
            key.subkeys = {"Subkey1", "Subkey2", "Subkey3"};
            
            return key;
        }
        
        std::vector<Value> GetValues(const std::string& path) override {
            auto key = OpenKey(path);
            return key ? key->values : std::vector<Value>{};
        }
        
        std::vector<std::string> GetSubkeys(const std::string& path) override {
            auto key = OpenKey(path);
            return key ? key->subkeys : std::vector<std::string>{};
        }
        
        bool CreateKey(const std::string& path) override {
            std::cout << "Creating key: " << path << std::endl;
            return true;
        }
        
        bool DeleteKey(const std::string& path) override {
            std::cout << "Deleting key: " << path << std::endl;
            return true;
        }
        
        bool SetValue(const std::string& path, const Value& value) override {
            std::cout << "Setting value: " << path << "\\" << value.name << std::endl;
            return true;
        }
        
        bool DeleteValue(const std::string& path, const std::string& valueName) override {
            std::cout << "Deleting value: " << path << "\\" << valueName << std::endl;
            return true;
        }
    };
    
    return std::make_unique<StubRegistryManager>();
}

} // namespace registry
