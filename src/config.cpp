#include "config.h"
#include <fstream>
#include "logger.h"

 Config::Config(const std::string& configFilePath)
 {
    parse(configFilePath);
 }

Config& Config::getInstance(const std::string& configFilePath)
{
    static Config instance(configFilePath) ;
    return instance;
}


 void Config::parse(const std::string& configFilePath)
 {
    std::ifstream file(configFilePath);
    if(!file.is_open()) {
    Logger::getInstance().log(Logger::Level::ERROR, "Could not open config file");
    return ;
    }

    std::string line;
    while(std::getline(file, line)) {
        auto eqaulToPosition = line.find('=');
        {
            if(line.substr(0, eqaulToPosition) == "port") 
            {
                m_port = std::stoi(line.substr(eqaulToPosition+1)) ;
            }
            else if (line.substr(0, eqaulToPosition) == "thread_count") 
            {
                m_threadCount = std::stoi(line.substr(eqaulToPosition+1)) ;
            }
            else if (line.substr(0, eqaulToPosition) == "root_directory") 
            {
                m_rootDirectory = line.substr(eqaulToPosition+1) ;
            }
            else
            {
                Logger::getInstance().log(Logger::Level::INFO, "More keys present in config file");

            }
        }
    }
 }

int Config::getPort() const
{
   return m_port;
} 

std::string Config::getRootDirectory() const
{
    return m_rootDirectory;
} 

int Config::getThreadCount() const
{
    return m_threadCount;
}