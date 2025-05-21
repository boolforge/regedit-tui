#include "registry_manager.h"

#ifdef PLATFORM_WINDOWS
#include "windows_registry_manager.h"
#endif

namespace registry {

// Factory method implementation
std::unique_ptr<RegistryManager> RegistryManager::Create() {
#ifdef PLATFORM_WINDOWS
    return std::make_unique<WindowsRegistryManager>();
#else
    // Return stub implementation for non-Windows platforms
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
#endif
}

} // namespace registry
