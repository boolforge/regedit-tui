#pragma once

#include <string>
#include <vector>
#include <memory>
#include <variant>
#include <optional>

namespace registry {

// Registry value types
enum class ValueType {
    REG_NONE,
    REG_SZ,
    REG_EXPAND_SZ,
    REG_BINARY,
    REG_DWORD,
    REG_DWORD_BIG_ENDIAN,
    REG_LINK,
    REG_MULTI_SZ,
    REG_RESOURCE_LIST,
    REG_QWORD,
    UNKNOWN
};

// Registry value data
using ValueData = std::variant<
    std::monostate,           // REG_NONE
    std::string,              // REG_SZ, REG_EXPAND_SZ
    std::vector<uint8_t>,     // REG_BINARY
    uint32_t,                 // REG_DWORD, REG_DWORD_BIG_ENDIAN
    uint64_t,                 // REG_QWORD
    std::vector<std::string>  // REG_MULTI_SZ
>;

// Registry value
struct Value {
    std::string name;
    ValueType type;
    ValueData data;
};

// Registry key
struct Key {
    std::string name;
    std::string path;
    std::vector<Value> values;
    std::vector<std::string> subkeys;
};

// Registry manager interface
class RegistryManager {
public:
    virtual ~RegistryManager() = default;

    // Open a registry key
    virtual std::optional<Key> OpenKey(const std::string& path) = 0;
    
    // Get values for a key
    virtual std::vector<Value> GetValues(const std::string& path) = 0;
    
    // Get subkeys for a key
    virtual std::vector<std::string> GetSubkeys(const std::string& path) = 0;
    
    // Create a new key
    virtual bool CreateKey(const std::string& path) = 0;
    
    // Delete a key
    virtual bool DeleteKey(const std::string& path) = 0;
    
    // Set a value
    virtual bool SetValue(const std::string& path, const Value& value) = 0;
    
    // Delete a value
    virtual bool DeleteValue(const std::string& path, const std::string& valueName) = 0;
    
    // Convert value type to string
    static std::string ValueTypeToString(ValueType type);
    
    // Convert value data to string representation
    static std::string ValueDataToString(const Value& value);
    
    // Factory method to create platform-specific registry manager
    static std::unique_ptr<RegistryManager> Create();
};

} // namespace registry
