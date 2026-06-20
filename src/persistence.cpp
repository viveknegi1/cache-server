#include "persistence.h"
#include <fstream>
#include "logger.h"

Persistence::Persistence(CacheStore& cacheStoreObj) : m_cacheStore(cacheStoreObj)
{


}

void Persistence::save(const std::string& filePath)
{
    //get cacheSnapShot
    auto cacheStoreSnapshot = m_cacheStore.getSnapShot();
    std::ofstream file(filePath, std::ios::binary);
    for(const auto& cachedData : cacheStoreSnapshot)
    {
        // write key length
        uint32_t length = cachedData.key.size(); 
        file.write(reinterpret_cast<const char*>(&length), sizeof(length));
        file.write(cachedData.key.data(), length);

        // write value length
        length = cachedData.value.size(); 
        file.write(reinterpret_cast<const char*>(&length), sizeof(length));
        file.write(cachedData.value.data(), length);

        // write timestamp
        file.write(reinterpret_cast<const char*>(&cachedData.hasExpiry), sizeof(cachedData.hasExpiry));
        // convert to int
        std::chrono::seconds::rep expiryTimeInInteger = std::chrono::duration_cast<std::chrono::seconds>(cachedData.expiryTime.time_since_epoch()).count();
        file.write(reinterpret_cast<const char*>(&expiryTimeInInteger), sizeof(expiryTimeInInteger));
    
    }

    file.close();

}

void Persistence::load(const std::string& filePath)
{

    CacheStore::CacheSnapShot snapshot;
   
    std::ifstream file(filePath, std::ios::binary);

    if (!file.is_open()) {
        Logger::getInstance().log(Logger::Level::ERROR, "Unable to open file");
        return ;
    }

    uint32_t len; 

    // The loop works exactly the same
    while (file.read(reinterpret_cast<char*>(&len), sizeof(len))) {
        
        // Read Key 
        std::string key(len, '\0'); 
        file.read(&key[0], len);    
        snapshot.key = key ;

        // Read value
        file.read(reinterpret_cast<char*>(&len), sizeof(len));
        std::string value(len, '\0');
        file.read(&value[0], len);
        snapshot.value = value ;

        // Read has expiry
        bool hasExpiry;
        file.read(reinterpret_cast<char*>(&hasExpiry), sizeof(hasExpiry)); 
        snapshot.hasExpiry = hasExpiry ;

        // Read expiry time
        decltype(std::chrono::seconds().count()) secondsValue;
        file.read(reinterpret_cast<char*>(&secondsValue), sizeof(secondsValue));
        auto expiryTime = std::chrono::system_clock::time_point(
            std::chrono::seconds(secondsValue));
        snapshot.expiryTime = expiryTime ;
        
        m_cacheStore.restoreEntry(snapshot);
    
    }
    file.close();  
}