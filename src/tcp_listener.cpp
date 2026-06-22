#include "tcp_listener.h"

#include "logger.h"
#include <netinet/in.h> // sockaddr_in struct
#include <sys/socket.h> //   socket(), bind(), listen(), accept()
#include <unistd.h> // close()

TcpListener::TcpListener(int port) {
    m_port = port;
    m_socket = -1; // No socket created yet
}

bool TcpListener::initialize() {

    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == -1) 
    {
        Logger::getInstance().log(Logger::Level::ERROR, "Failed to create socket");
        return false;
    }
    /**
    I know there might be a socket in TIME_WAIT on this port, but ignore the standard safety timer. 
    Let my new server instance bind to this port and start listening immediately.
    */
    int opt = 1;
    setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    return true ;
}

bool TcpListener::bindSocket()
{
    sockaddr_in in_sockaddrStruct ; 
    in_sockaddrStruct.sin_family = AF_INET ;
    in_sockaddrStruct.sin_port = htons(m_port) ; //converts byte order 
    in_sockaddrStruct.sin_addr.s_addr = INADDR_ANY ;

    int result = bind(m_socket , (sockaddr*)&in_sockaddrStruct, sizeof(in_sockaddrStruct));
    if( result == -1)
    {
        Logger::getInstance().log(Logger::Level::ERROR, "Failed to bind the socket");
        return false;
    }

    return true ;

}

bool TcpListener::startListening()
{
    int backlog = 10 ; // queue size 
    int result = listen(m_socket, backlog);
    if( result == -1)
    {
        Logger::getInstance().log(Logger::Level::ERROR, "Failed to start listening");
        return false;
    }

    return true ;
}

int TcpListener::acceptConnection()
{

    sockaddr_in clientAddr;
    socklen_t clientSize = sizeof(clientAddr);

    int clientFd = accept(m_socket, (sockaddr*)&clientAddr, &clientSize);
    if(clientFd == -1)
    {
        Logger::getInstance().log(Logger::Level::ERROR, "Failed to accept the client");
       
    }

    return clientFd;
}


TcpListener::~TcpListener()
{
    if(m_socket != -1)
    {
        close(m_socket);
    }
    m_socket = -1;  
}


