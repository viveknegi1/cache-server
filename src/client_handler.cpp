#include "client_handler.h"

#include <charconv>
#include "command_parser.h"
#include "logger.h"
#include "response_builder.h"
#include <sys/socket.h>

 ClientHandler::ClientHandler(int clientFd ,CacheStore& cacheStoreRef , Persistence& persistenceRef) : m_cacheStore(cacheStoreRef), m_persistence(persistenceRef)
 {
    m_clientFd = clientFd ;
 }

std::string ClientHandler::handleSet(std::string key, std::string value , std::optional<int> ttlSeconds)
{
    m_cacheStore.set(std::move(key), std::move(value), ttlSeconds);
    return ResponseBuilder::success();  
}

std::string ClientHandler::handleGet(const std::string& key)
{
    auto result = m_cacheStore.get(key);
    if(result.has_value()) 
    {
        return ResponseBuilder::successWithValues(result.value());
    }
    return ResponseBuilder::notFound();
}

std::string ClientHandler::handleDel(std::string key)
{
    auto result = m_cacheStore.del(key);
    if(result) 
    {
        return ResponseBuilder::success();
    }
    return ResponseBuilder::error("Key wasn't deleted");
}

std::string ClientHandler::handleFlush()
{
    m_cacheStore.flush();
    return ResponseBuilder::success();
}

std::string ClientHandler::handleGetKeys()
{
    auto keys = m_cacheStore.keys();
    return ResponseBuilder::keys(keys);
}

std::string ClientHandler::handleExists(const std::string& key)
{
    auto result = m_cacheStore.exists(key);
    return ResponseBuilder::result(result);
}
   
void ClientHandler::run()
{
    while(true)
    {
        // Receive input from client and parse it
        char buffer[4096]; 
        int bytesReceived = recv(m_clientFd, buffer, sizeof(buffer), 0); 
        if(bytesReceived == -1)
        {
            Logger::getInstance().log(Logger::Level::ERROR, "Insufficient Data received from client request");
            return ;

        }
        else if (bytesReceived == 0)
        {
            Logger::getInstance().log(Logger::Level::ERROR, "Client Disconnected");
            return ;
        } 
        else
        {
            std::string bufferString(buffer, bytesReceived);
            auto parsedResult = CommandParser::parse(bufferString);
            auto methodType = parsedResult.type ;
            auto argumentList = parsedResult.args ;
            std::string response ;
            switch (methodType)
            {
                case  CommandParser::CommandType::SET:
                    {
                        if(argumentList.empty() || argumentList.size() == 1)
                        {
                            response =  ResponseBuilder::error("Arguments are fewer than expected");
                        
                        }
                        else
                        {
                            auto key = argumentList.at(0);
                            auto value = argumentList.at(1);
                            std::optional<int> ttl = std::nullopt;
                            if (argumentList.size() > 2)
                            {
                                int ttlInt ; 
                                auto str = argumentList[2];
                                auto ttlString = std::from_chars(str.data(), str.data() + str.size(), ttlInt);
                                if (ttlString.ec == std::errc())
                                { 
                                    ttl = ttlInt; 
                                }
                            }

                            response = handleSet(key, value, ttl);
                        }

                        break;    
                    }

                case CommandParser::CommandType::GET:
                {
                    if(argumentList.empty())
                    {
                        response =  ResponseBuilder::error("Arguments are fewer than expected");
                        
                    }
                    else
                    {
                        auto key = argumentList.at(0);
                        response =  handleGet(key);
                    }
                    break;
                }

                case CommandParser::CommandType::DEL:
                {
                    if(argumentList.empty())
                    {
                        response =  ResponseBuilder::error("Arguments are fewer than expected");    
                    }
                    else
                    {
                        auto key = argumentList.at(0);
                        response =  handleDel(key);
                    }
                    break;
                }

                case CommandParser::CommandType::EXISTS:
                {
                    if(argumentList.empty())
                    {
                        response =  ResponseBuilder::error("Arguments are fewer than expected");
                        
                    }
                    else
                    {
                        auto key = argumentList.at(0);
                        response =  handleExists(key);
                    }
                    break;
                }
                case CommandParser::CommandType::FLUSH:
                    response =  handleFlush();
                    break;
                case CommandParser::CommandType::SAVE:
                {
                    m_persistence.save("cache.dat");  
                    response = ResponseBuilder::success();
                    break;
                }
                case CommandParser::CommandType::KEYS:
                    response = handleGetKeys();
                    break;
                case CommandParser::CommandType::UNKNOWN:
                    response = ResponseBuilder::error("Unknown command");
                    break;
                default: 
                    response = ResponseBuilder::error("Unknown command");
                    break;

            }
          // Send data to client  
          auto byteSent = send(m_clientFd, response.c_str(), response.size(), 0);
          if (byteSent == -1) 
          {
                Logger::getInstance().log(Logger::Level::ERROR, "Socket Error");
                return ;
          } 
          else if (byteSent == 0) 
          {
                Logger::getInstance().log(Logger::Level::ERROR, "Client disconnected. Connection closed");
                return ;

          } 
          else 
          {
                auto totalBytesSent = std::to_string(byteSent);
                Logger::getInstance().log(Logger::Level::INFO, "Sent " + totalBytesSent + " bytes");
          }
            
        }
    }
    
}


