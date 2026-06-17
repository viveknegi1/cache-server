 #pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <functional>

class ThreadPool
{
    private:
       std::vector<std::thread> m_threads ;   // vector of worker threads
       std::queue<std::function<void()>> m_taskQueue; // queue of  tasks. 
       std::mutex m_mutex ;     // protects the queue
       std::condition_variable m_condition ; // wakes sleeping workers
       bool m_stop = false;     // tells workers to shut down

    public:
    ThreadPool(int threadCount);
    void enqueue(std::function<void()> task) ; // Takes a tasks and add it in queue. 
    public: ~ThreadPool()  ;    
};