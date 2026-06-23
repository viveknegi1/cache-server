#include "command_parser.h"

#include <string>

CommandParser::CommandType stringToCommandType(const std::string& commandString)
{
    if(commandString == "SET")
    { 
        return CommandParser::CommandType::SET;
    }
    else if(commandString == "GET") 
    {
        return CommandParser::CommandType::GET;
    }
    else if(commandString == "DEL") 
    {
        return CommandParser::CommandType::DEL;
    }
    else if(commandString == "EXISTS") 
    {
        return CommandParser::CommandType::EXISTS;
    }
    else if(commandString == "KEYS") 
    {
        return CommandParser::CommandType::KEYS;
    }
    else if(commandString == "FLUSH") 
    {
        return CommandParser::CommandType::FLUSH;
    }
    else if(commandString == "SAVE") 
    {
        return CommandParser::CommandType::SAVE;
    }
    else 
    {
        return CommandParser::CommandType::UNKNOWN;
    }
} 

CommandParser::ParsedCommand CommandParser::parse(const std::string& rawCommandString)
 {
    CommandParser::ParsedCommand parsedResult;
    std::string cleanedString = rawCommandString;
    // Raw strings can have '\r\n at the end. So cleaning up them for better parsing
    while (!cleanedString.empty() && (cleanedString.back() == '\n' || cleanedString.back() == '\r')) 
    {
        cleanedString.pop_back();
    }

    bool isOnlySpaces = (cleanedString.find_first_not_of(" \t") == std::string::npos);

    size_t firstSpacePos = cleanedString.find(' ');
    std::string commandString = cleanedString.substr(0, firstSpacePos);
    auto command = stringToCommandType(commandString);

    if (isOnlySpaces) {
        parsedResult.type = command;
        return parsedResult ;
    }


    if(firstSpacePos == std::string::npos) 
    {
        // This command have no arguments. So return early.
        parsedResult.type = command;
        return parsedResult;  
    }

    std::string argumentSubstring = cleanedString.substr(firstSpacePos + 1);
    std::vector<std::string> argumentList;
    std::string currentArgument;
    for( auto& character : argumentSubstring)
    {
        if(character == ' ')
        {
           argumentList.push_back(currentArgument) ;
           currentArgument = "";
        }
        else
        {
            currentArgument += character ;
        }
    }
    // We have recieved final 'space' . Push remaining argument to the vector.
    argumentList.push_back(currentArgument) ;
    parsedResult.type = command ;
    parsedResult.args = argumentList ;

    return parsedResult;
 }
