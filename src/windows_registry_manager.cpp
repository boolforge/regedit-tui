#ifdef PLATFORM_WINDOWS
#include "windows_registry_manager.h"
#include <windows.h>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace registry {

WindowsRegistryManager::WindowsRegistryManager() {
    // Constructor implementation
}

WindowsRegistryManager::~WindowsRegistryManager() {
    // Destructor implementation
}

std::optional<Key> WindowsRegistryManager::OpenKey(const std::string& path) {
    auto [hRootKey, subKey] = ParseRegistryPath(path);
    if (hRootKey == NULL) {
        return std::nullopt;
    }

    HKEY hKey;
    LONG result = RegOpenKeyExA(hRootKey, subKey.c_str(), 0, KEY_READ, &hKey);
    if (result != ERROR_SUCCESS) {
        return std::nullopt;
    }

    Key key;
    key.name = path.substr(path.find_last_of('\\') + 1);
    key.path = path;
    key.values = GetValues(path);
    key.subkeys = GetSubkeys(path);

    RegCloseKey(hKey);
    return key;
}

std::vector<Value> WindowsRegistryManager::GetValues(const std::string& path) {
    auto [hRootKey, subKey] = ParseRegistryPath(path);
    if (hRootKey == NULL) {
        return {};
    }

    HKEY hKey;
    LONG result = RegOpenKeyExA(hRootKey, subKey.c_str(), 0, KEY_READ, &hKey);
    if (result != ERROR_SUCCESS) {
        return {};
    }

    std::vector<Value> values;
    
    char valueName[256];
    DWORD valueNameSize = sizeof(valueName);
    DWORD valueType;
    DWORD valueIndex = 0;

    while (RegEnumValueA(hKey, valueIndex, valueName, &valueNameSize, NULL, &valueType, NULL, NULL) == ERROR_SUCCESS) {
        Value value;
        value.name = valueName;
        value.type = GetValueType(valueType);
        value.data = ReadValueData(hKey, valueName, value.type);
        
        values.push_back(value);
        
        valueNameSize = sizeof(valueName);
        valueIndex++;
    }

    RegCloseKey(hKey);
    return values;
}

std::vector<std::string> WindowsRegistryManager::GetSubkeys(const std::string& path) {
    auto [hRootKey, subKey] = ParseRegistryPath(path);
    if (hRootKey == NULL) {
        return {};
    }

    HKEY hKey;
    LONG result = RegOpenKeyExA(hRootKey, subKey.c_str(), 0, KEY_READ, &hKey);
    if (result != ERROR_SUCCESS) {
        return {};
    }

    std::vector<std::string> subkeys;
    
    char keyName[256];
    DWORD keyNameSize = sizeof(keyName);
    DWORD keyIndex = 0;

    while (RegEnumKeyExA(hKey, keyIndex, keyName, &keyNameSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
        subkeys.push_back(keyName);
        keyNameSize = sizeof(keyName);
        keyIndex++;
    }

    RegCloseKey(hKey);
    return subkeys;
}

bool WindowsRegistryManager::CreateKey(const std::string& path) {
    auto [hRootKey, subKey] = ParseRegistryPath(path);
    if (hRootKey == NULL) {
        return false;
    }

    HKEY hKey;
    DWORD disposition;
    LONG result = RegCreateKeyExA(hRootKey, subKey.c_str(), 0, NULL, 0, KEY_WRITE, NULL, &hKey, &disposition);
    if (result != ERROR_SUCCESS) {
        return false;
    }

    RegCloseKey(hKey);
    return true;
}

bool WindowsRegistryManager::DeleteKey(const std::string& path) {
    auto [hRootKey, subKey] = ParseRegistryPath(path);
    if (hRootKey == NULL) {
        return false;
    }

    LONG result = RegDeleteKeyA(hRootKey, subKey.c_str());
    return result == ERROR_SUCCESS;
}

bool WindowsRegistryManager::SetValue(const std::string& path, const Value& value) {
    auto [hRootKey, subKey] = ParseRegistryPath(path);
    if (hRootKey == NULL) {
        return false;
    }

    HKEY hKey;
    LONG result = RegOpenKeyExA(hRootKey, subKey.c_str(), 0, KEY_WRITE, &hKey);
    if (result != ERROR_SUCCESS) {
        return false;
    }

    bool success = false;
    DWORD winType = GetWinType(value.type);

    switch (value.type) {
        case ValueType::REG_SZ:
        case ValueType::REG_EXPAND_SZ: {
            const std::string& str = std::get<std::string>(value.data);
            result = RegSetValueExA(hKey, value.name.c_str(), 0, winType, 
                                   reinterpret_cast<const BYTE*>(str.c_str()), 
                                   static_cast<DWORD>(str.size() + 1));
            success = (result == ERROR_SUCCESS);
            break;
        }
        case ValueType::REG_BINARY: {
            const auto& data = std::get<std::vector<uint8_t>>(value.data);
            result = RegSetValueExA(hKey, value.name.c_str(), 0, winType, 
                                   data.data(), 
                                   static_cast<DWORD>(data.size()));
            success = (result == ERROR_SUCCESS);
            break;
        }
        case ValueType::REG_DWORD:
        case ValueType::REG_DWORD_BIG_ENDIAN: {
            uint32_t data = std::get<uint32_t>(value.data);
            result = RegSetValueExA(hKey, value.name.c_str(), 0, winType, 
                                   reinterpret_cast<const BYTE*>(&data), 
                                   sizeof(data));
            success = (result == ERROR_SUCCESS);
            break;
        }
        case ValueType::REG_QWORD: {
            uint64_t data = std::get<uint64_t>(value.data);
            result = RegSetValueExA(hKey, value.name.c_str(), 0, winType, 
                                   reinterpret_cast<const BYTE*>(&data), 
                                   sizeof(data));
            success = (result == ERROR_SUCCESS);
            break;
        }
        case ValueType::REG_MULTI_SZ: {
            const auto& strings = std::get<std::vector<std::string>>(value.data);
            std::string combined;
            for (const auto& str : strings) {
                combined += str;
                combined.push_back('\0');
            }
            combined.push_back('\0');
            
            result = RegSetValueExA(hKey, value.name.c_str(), 0, winType, 
                                   reinterpret_cast<const BYTE*>(combined.c_str()), 
                                   static_cast<DWORD>(combined.size()));
            success = (result == ERROR_SUCCESS);
            break;
        }
        default:
            success = false;
            break;
    }

    RegCloseKey(hKey);
    return success;
}

bool WindowsRegistryManager::DeleteValue(const std::string& path, const std::string& valueName) {
    auto [hRootKey, subKey] = ParseRegistryPath(path);
    if (hRootKey == NULL) {
        return false;
    }

    HKEY hKey;
    LONG result = RegOpenKeyExA(hRootKey, subKey.c_str(), 0, KEY_WRITE, &hKey);
    if (result != ERROR_SUCCESS) {
        return false;
    }

    result = RegDeleteValueA(hKey, valueName.c_str());
    RegCloseKey(hKey);
    
    return result == ERROR_SUCCESS;
}

// Helper methods
HKEY WindowsRegistryManager::GetRootKeyHandle(const std::string& rootKeyName) {
    if (rootKeyName == "HKEY_CLASSES_ROOT" || rootKeyName == "HKCR") {
        return HKEY_CLASSES_ROOT;
    } else if (rootKeyName == "HKEY_CURRENT_USER" || rootKeyName == "HKCU") {
        return HKEY_CURRENT_USER;
    } else if (rootKeyName == "HKEY_LOCAL_MACHINE" || rootKeyName == "HKLM") {
        return HKEY_LOCAL_MACHINE;
    } else if (rootKeyName == "HKEY_USERS" || rootKeyName == "HKU") {
        return HKEY_USERS;
    } else if (rootKeyName == "HKEY_CURRENT_CONFIG" || rootKeyName == "HKCC") {
        return HKEY_CURRENT_CONFIG;
    }
    return NULL;
}

std::pair<HKEY, std::string> WindowsRegistryManager::ParseRegistryPath(const std::string& path) {
    size_t pos = path.find('\\');
    if (pos == std::string::npos) {
        return {GetRootKeyHandle(path), ""};
    }
    
    std::string rootKeyName = path.substr(0, pos);
    std::string subKey = path.substr(pos + 1);
    
    return {GetRootKeyHandle(rootKeyName), subKey};
}

ValueType WindowsRegistryManager::GetValueType(DWORD winType) {
    switch (winType) {
        case REG_NONE: return ValueType::REG_NONE;
        case REG_SZ: return ValueType::REG_SZ;
        case REG_EXPAND_SZ: return ValueType::REG_EXPAND_SZ;
        case REG_BINARY: return ValueType::REG_BINARY;
        case REG_DWORD: return ValueType::REG_DWORD;
        case REG_DWORD_BIG_ENDIAN: return ValueType::REG_DWORD_BIG_ENDIAN;
        case REG_LINK: return ValueType::REG_LINK;
        case REG_MULTI_SZ: return ValueType::REG_MULTI_SZ;
        case REG_RESOURCE_LIST: return ValueType::REG_RESOURCE_LIST;
        case REG_QWORD: return ValueType::REG_QWORD;
        default: return ValueType::UNKNOWN;
    }
}

DWORD WindowsRegistryManager::GetWinType(ValueType type) {
    switch (type) {
        case ValueType::REG_NONE: return REG_NONE;
        case ValueType::REG_SZ: return REG_SZ;
        case ValueType::REG_EXPAND_SZ: return REG_EXPAND_SZ;
        case ValueType::REG_BINARY: return REG_BINARY;
        case ValueType::REG_DWORD: return REG_DWORD;
        case ValueType::REG_DWORD_BIG_ENDIAN: return REG_DWORD_BIG_ENDIAN;
        case ValueType::REG_LINK: return REG_LINK;
        case ValueType::REG_MULTI_SZ: return REG_MULTI_SZ;
        case ValueType::REG_RESOURCE_LIST: return REG_RESOURCE_LIST;
        case ValueType::REG_QWORD: return REG_QWORD;
        default: return REG_NONE;
    }
}

ValueData WindowsRegistryManager::ReadValueData(HKEY hKey, const std::string& valueName, ValueType type) {
    DWORD dataSize = 0;
    DWORD winType = GetWinType(type);
    
    // Get the data size
    RegQueryValueExA(hKey, valueName.c_str(), NULL, &winType, NULL, &dataSize);
    
    switch (type) {
        case ValueType::REG_NONE:
            return std::monostate{};
            
        case ValueType::REG_SZ:
        case ValueType::REG_EXPAND_SZ: {
            std::vector<char> buffer(dataSize);
            RegQueryValueExA(hKey, valueName.c_str(), NULL, &winType, reinterpret_cast<BYTE*>(buffer.data()), &dataSize);
            return std::string(buffer.data());
        }
            
        case ValueType::REG_BINARY: {
            std::vector<uint8_t> buffer(dataSize);
            RegQueryValueExA(hKey, valueName.c_str(), NULL, &winType, buffer.data(), &dataSize);
            return buffer;
        }
            
        case ValueType::REG_DWORD:
        case ValueType::REG_DWORD_BIG_ENDIAN: {
            uint32_t value = 0;
            RegQueryValueExA(hKey, valueName.c_str(), NULL, &winType, reinterpret_cast<BYTE*>(&value), &dataSize);
            return value;
        }
            
        case ValueType::REG_QWORD: {
            uint64_t value = 0;
            RegQueryValueExA(hKey, valueName.c_str(), NULL, &winType, reinterpret_cast<BYTE*>(&value), &dataSize);
            return value;
        }
            
        case ValueType::REG_MULTI_SZ: {
            std::vector<char> buffer(dataSize);
            RegQueryValueExA(hKey, valueName.c_str(), NULL, &winType, reinterpret_cast<BYTE*>(buffer.data()), &dataSize);
            
            std::vector<std::string> strings;
            const char* ptr = buffer.data();
            while (*ptr) {
                strings.push_back(ptr);
                ptr += strlen(ptr) + 1;
            }
            
            return strings;
        }
            
        default:
            return std::monostate{};
    }
}

} // namespace registry
#endif
