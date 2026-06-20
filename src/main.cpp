#include "logger.h"
#include "config.h"
#include <string>
#include "cache_store.h"
#include "persistence.h"

int main()
{

Config::getInstance("config.ini");
Logger::getInstance().log(Logger::Level::INFO, "Cache Server Started");

CacheStore cacheStoreObj ;
Persistence persistenceObj(cacheStoreObj) ;

std::string key = "Name" ;
std::string value = "Vivek" ;
std::optional<int> ttl = 60 ;
std::string ttlInString = std::to_string(ttl.value());
Logger::getInstance().log(Logger::Level::INFO, "Storing Key " + key + " with value as " + value + " and TTL as " + ttlInString);
cacheStoreObj.set(key, value, ttl);


key = "Age" ;
value = "30" ;
ttl = 60 ;
ttlInString = std::to_string(ttl.value());
Logger::getInstance().log(Logger::Level::INFO, "Storing Key " + key + " with value as " + value + " and TTL as " + ttlInString);
cacheStoreObj.set(key, value, ttl);

key = "height" ;
value = "180" ;
ttl = std::nullopt ;
Logger::getInstance().log(Logger::Level::INFO, "Storing Key " + key + " with value as " + value + " and No TTL");
cacheStoreObj.set(key, value, ttl);

persistenceObj.save("cache.dat");

CacheStore secondCacheStoreObj ;
Persistence secondPersistenceObj(secondCacheStoreObj);
secondPersistenceObj.load("cache.dat");
auto keys = secondCacheStoreObj.keys();
for(const auto& key : keys)
{
    Logger::getInstance().log(Logger::Level::INFO, "Printing Keys from Second Cache Store " + key);

    auto resultValue = secondCacheStoreObj.get(key);
    if(resultValue != std::nullopt)
    {
        Logger::getInstance().log(Logger::Level::INFO, "Printing value of key " + key + " Value : " + resultValue.value() + " from Second Cache Store");

    }

}




return 0;

}