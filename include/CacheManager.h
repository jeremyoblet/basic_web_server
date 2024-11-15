#pragma once
#include <string>
#include <unordered_map>

class CacheManager
{
public:
    CacheManager(const CacheManager&) = delete;
    CacheManager& operator=(const CacheManager&) = delete;

    static CacheManager& GetInstance() {
        static CacheManager instance;
        return instance;
    }

    const std::string& GetFile(const std::string& path);
    const std::string& GetCache(const std::string& key) ;
    void SetCache(const std::string& key, const std::string& value);

private:
    CacheManager() = default;  // Private constructor
    ~CacheManager() = default;


    std::unordered_map<std::string, std::string> cache;
};