#pragma once
#include <unordered_map>
#include <mutex>
#include <vector>
#include <string>
#include <optional>
#include <chrono>

class CacheStore{

private:
    struct CacheEntry 
    {
        std::string value;
        std::chrono::steady_clock::time_point expiryTime;
        bool hasExpiry;  // some keys might never expire
    };

    std::unordered_map<std::string, CacheEntry> m_store;
    mutable std::mutex m_mutex;
    
public:

    void set(std::string key, std::string value , std::optional<int> ttlSeconds = std::nullopt);
    std::optional<std::string> get(const std::string& key) const;
    bool del(const std::string& key) ; // should return whether something was actually deleted
    bool exists(const std::string& key) const ;
    std::vector<std::string> keys() const ;
    void flush() ;
    void removeExpiredEntries();
}; 