#pragma once

#include<string>

class Config{

public:
    static Config&  getInstance(const std::string& configFilePath  = " ");
    std::string getRootDirectory() const ; 

    int getPort() const ; 
    int getThreadCount() const ;
    int getSweepInterval() const;

private:
    int m_port;
    int m_threadCount;
    std::string m_rootDirectory;
    int m_sweepInterval;

    void parse(const std::string& configFilePath);
    Config(const std::string& configFilePath);
    Config(const Config&) = delete; // delete copy constructor
    Config& operator=(const Config&) = delete; // delete operator = 

};