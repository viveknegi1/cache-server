#pragma once
#include <unordered_map>
#include <mutex>
#include <vector>
#include <string>
#include <optional>

class CacheStore{

private:
    std::unordered_map<std::string, std::string> m_store;
    mutable std::mutex m_mutex;
   
public:

    void set(std::string key, const std::string& value);
    std::optional<std::string> get(const std::string& key) const; 
    bool del(const std::string& key) ; // should return whether something was actually deleted
    bool exists(const std::string& key) const ;
    std::vector<std::string> keys() const ;
    void flush() ;
}; 