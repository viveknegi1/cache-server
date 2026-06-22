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
                // The thread will sleep for up to m_sweepIntervalSeconds.
                // 1. SHUTDOWN CASE: If the destructor sets m_stop to true and notifies us, 
                //    the lambda returns true, waking the thread up INSTANTLY (early exit).
                // 2. RUNNING CASE: If no shutdown happens, the 5-second timer naturally expires. 
                //    Even though the lambda returns false, the timeout forces the wait to end. 
                m_condition.wait_for(lock, std::chrono::seconds(m_sweepIntervalSeconds), [this]() {
                    return m_stop.load(); // read its current value safely across threads.
                 });
                
                // If we woke up because a shutdown was triggered, exit the thread immediately
                if(m_stop) return;

                // If we reached here, the 5-second timer expired naturally. Run the cleanup.
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

