#pragma once

#include <atomic>
#include "cache_store.h"
#include <condition_variable>
#include <mutex>
#include <thread>

class TtlManager {

private:
    CacheStore& m_cacheStore; 
    std::thread m_thread ; 
    std::atomic<bool> m_stop ; 
    int m_sweepIntervalSeconds ;
    std::mutex m_mutex;
    std::condition_variable m_condition;

public: 
    TtlManager(CacheStore& cacheStoreObj , int sweepIntervalSeconds);
    ~TtlManager();

};