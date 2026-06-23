
#include <atomic> 
#include "cache_store.h"
#include "client_handler.h"
#include "config.h"
#include <csignal> 
#include "logger.h"
#include "persistence.h"
#include "tcp_listener.h"
#include "thread_pool.h"
#include "ttl_manager.h"
#include <string>

// Global flag — signal handler sets this to true
std::atomic<bool> g_running = true;

// Signal handler — called when Ctrl+C is pressed
void signalHandler(int) 
{
    g_running = false;
}

int main()
{
    CacheStore cacheStoreObj ;
    
    Config::getInstance("config.ini");
    int sweepInterval  = Config::getInstance().getSweepInterval();
    TtlManager ttlManagerObj(cacheStoreObj,sweepInterval);
    Persistence persistenceObj(cacheStoreObj);
    persistenceObj.load("cache.dat");

    std::signal(SIGINT, signalHandler);
    int port = Config::getInstance("config.ini").getPort() ;
    // Create listener
    TcpListener tcpListenerObj(port) ;
    if(!tcpListenerObj.initialize())
    {
        Logger::getInstance().log(Logger::Level::ERROR, "TCPListener inialization failed");
        return -1;
    }

    if(!tcpListenerObj.bindSocket())
    {
        Logger::getInstance().log(Logger::Level::ERROR, "TCPListener socket binding failed ");
        return -1;
    }

    if(!tcpListenerObj.startListening())
    {
        Logger::getInstance().log(Logger::Level::ERROR, "TCPListener Listener binding failed ");
        return -1;
    }

    Logger::getInstance().log(Logger::Level::INFO, "Server listening on port " + std::to_string(port));

      // Connect with the server
    int threadCount = Config::getInstance().getThreadCount() ;
    ThreadPool pool(threadCount);
    while(g_running) 
    {
        int clientFd = tcpListenerObj.acceptConnection();
        if(clientFd == -1) continue;
        // Capture the client file descriptor and hand off the workload to the thread pool
        pool.enqueue([clientFd, &cacheStoreObj, &persistenceObj]() {  
        // The object is created and lives entirely within the worker thread
            ClientHandler clientHandlerObj(clientFd, cacheStoreObj, persistenceObj);
            clientHandlerObj.run();  
        });
    }

    persistenceObj.save("cache.dat");
    Logger::getInstance().log(Logger::Level::INFO, "Server Shutting Down");
    return 0;

}