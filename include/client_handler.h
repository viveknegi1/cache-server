#pragma once

#include "cache_store.h"
#include "persistence.h"
#include <string>
#include <optional>

class ClientHandler{

private:
    int m_clientFd; 
    CacheStore& m_cacheStore;
    Persistence& m_persistence;
    std::string handleSet(std::string key, std::string value , std::optional<int> ttlSeconds = std::nullopt);
    std::string handleGet(const std::string& key);
    std::string handleDel(std::string key);
    std::string handleFlush();
    std::string handleGetKeys();
    std::string handleExists(const std::string& key);

public:
    ClientHandler(int clientFd ,CacheStore& cacheStoreRef , Persistence& persistenceRef);
    void run();

};
