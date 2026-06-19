#include "ttl_manager.h"
 #include <chrono>


TtlManager::TtlManager(CacheStore& cacheStoreObj , int sweepIntervalSeconds) : m_cacheStore(cacheStoreObj), m_sweepIntervalSeconds(sweepIntervalSeconds)
{
    m_stop = false;
    m_thread = std::thread([this]()   
        {
            while(!m_stop) 
            {
                
                std::unique_lock<std::mutex> lock(m_mutex);
                m_condition.wait_for(lock, std::chrono::seconds(m_sweepIntervalSeconds), [this]() {
                    return m_stop.load(); // read its current value safely across threads.
                 });
                if(m_stop) return;
                m_cacheStore.removeExpiredEntries();  
            }
        });
}

TtlManager::~TtlManager()
{

    m_stop = true;
    m_condition.notify_all();
    m_thread.join();

}

