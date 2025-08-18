#include "CacheManager.h"
#include <fstream>

const std::string& CacheManager::GetCache(const std::string& key)
{
    static const std::string empty;

#if __cplusplus >= 202002L
    if(cache.contains(key)) {
        return cache.at(key);
    }
#else
    if(cache.find(key) != cache.end()) {
        return cache.at(key);
    }
#endif

    std::ifstream file(key, std::ios::binary);
    if(!file.is_open()) {
        return empty;
    }

    const std::string file_data = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    std::pair<std::string, std::string> cache_entry (key, file_data);
    cache.insert(cache_entry);

    return cache.at(key);
}

const std::string& CacheManager::GetFile(const std::string& path) {
    return GetCache(path);
}