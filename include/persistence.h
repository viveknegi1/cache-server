#pragma once

#include "cache_store.h"
#include <string>

class Persistence{

    private:
        CacheStore& m_cacheStore ;

    public:
        Persistence(CacheStore& cacheStoreObj);
        void save(const std::string& filePath); 
        void load(const std::string& filePath);

};