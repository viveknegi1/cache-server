#include "thread_pool.h"
#include <sys/socket.h>


ThreadPool::ThreadPool(int threadCount)
{
    m_stop = false ;
    for(int i = 0; i < threadCount; i++)
    {
        // Create a thread and push it in the vector of current threads  
        //Thread will be initialised with lambda function.   
        m_threads.push_back(std::thread([this]()   
        {
            while(true) 
            {
                std::function<void()> task; // empty function object
                {
                    std::unique_lock<std::mutex> lock(m_mutex);
                    m_condition.wait(lock, [this]() 
                    {
                        return !m_taskQueue.empty() || m_stop;
                    });
                // if task queue is empty or m_stop is true return. 
                if(m_stop && m_taskQueue.empty()) 
                {
                    return;
                }
                // pick task from front task queue and start execution.
                task = m_taskQueue.front();
                m_taskQueue.pop();
            }
        // lock released here
        task();
        }
    }));
}

}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_taskQueue.push(task);
    }
    m_condition.notify_one();
}


ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_stop = true;
    }
    m_condition.notify_all();
    for( auto& thread: m_threads)
    {
        thread.join();
    }
}
