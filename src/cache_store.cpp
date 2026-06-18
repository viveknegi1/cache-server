#include "cache_store.h"

void CacheStore::set(std::string key, const std::string& value)
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_store[std::move(key)] = std::move(value);
    }
   
}

std::optional<std::string> CacheStore::get(const std::string& key) const
{
    
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_store.find(key);
        if(it != m_store.end()) {
            return it->second;
        }
    }
    return std::nullopt; 
} 

bool CacheStore::del(const std::string& key)
{
    size_t result ;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        result = m_store.erase(key);
            
    }
    return result > 0; 
}  
    
bool CacheStore::exists(const std::string& key) const
{
    bool keyExists = false ;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_store.find(key);
        if(it != m_store.end()) {
            keyExists = true;
        }
    }
    return keyExists;
} 

std::vector<std::string> CacheStore::keys() const
{
    std::vector<std::string> keys ;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (const auto& [key, value] : m_store) 
        {
             keys.push_back(key);
        }
    }
    return keys;
}
    
void CacheStore::flush()
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_store.clear();
    }
   
}
    