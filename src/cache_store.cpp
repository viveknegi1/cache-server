#include "cache_store.h"
#include <optional>

void CacheStore::set(std::string key, std::string value, std::optional<int> ttlSeconds)
{
    {   
        CacheStore::CacheEntry cacheData;
        std::lock_guard<std::mutex> lock(m_mutex);
        if(ttlSeconds != std::nullopt )
        {
            cacheData.expiryTime = std::chrono::steady_clock::now() + std::chrono::seconds(ttlSeconds.value()) ;
            cacheData.hasExpiry = true;
        }
        else
        {
            cacheData.hasExpiry = false;
        }
        cacheData.value = std::move(value);
        m_store[std::move(key)] = std::move(cacheData);
    
    }
        
}
   
std::optional<std::string> CacheStore::get(const std::string& key) const
{
    
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_store.find(key);
        if(it != m_store.end()) {
            if( it->second.hasExpiry == false || ( it->second.expiryTime > std::chrono::steady_clock::now()))
            {
                return it->second.value;
            }
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


void CacheStore::removeExpiredEntries()
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_store.begin();
        while(it != m_store.end()) 
        {
            if(it->second.hasExpiry == true && ( it->second.expiryTime < std::chrono::steady_clock::now())) 
            {
                it = m_store.erase(it);  // erase returns next valid iterator
            } 
            else 
            {
                ++it;  // only advance manually if we didn't erase
            }
        }
    }
}
  
    