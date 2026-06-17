#pragma once

class TcpListener {

private:
    int m_port;
    int m_socket;

public:
    TcpListener(int port);
    bool initialize();
    bool bindSocket();
    bool startListening();
    int acceptConnection();
    ~TcpListener();

};
