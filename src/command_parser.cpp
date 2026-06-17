#include "command_parser.h"
#include <string>

CommandParser::CommandType stringToCommandType(const std::string& commandString)
{
    if(commandString == "SET") return CommandParser::CommandType::SET;
    else if(commandString == "GET") return CommandParser::CommandType::GET;
    else if(commandString == "DEL") return CommandParser::CommandType::DEL;
    else if(commandString == "EXISTS") return CommandParser::CommandType::EXISTS;
    else if(commandString == "KEYS") return CommandParser::CommandType::KEYS;
    else if(commandString == "FLUSH") return CommandParser::CommandType::FLUSH;
    else if(commandString == "SAVE") return CommandParser::CommandType::SAVE;
    else return CommandParser::CommandType::UNKNOWN;
} 

CommandParser::ParsedCommand CommandParser::parse(const std::string& rawCommandString)
 {
    CommandParser::ParsedCommand parsedResult;
    std::string cleanedString = rawCommandString;
    while (!cleanedString.empty() && (cleanedString.back() == '\n' || cleanedString.back() == '\r')) {
        cleanedString.pop_back();
    }

    size_t firstSpacePos = cleanedString.find(' ');
    std::string commandString = cleanedString.substr(0, firstSpacePos);
    auto command = stringToCommandType(commandString);

    if(firstSpacePos == std::string::npos) {
    // no arguments
    parsedResult.type = command;
    return parsedResult;  // args is empty by default
    }

    std::string argumentSubstring = cleanedString.substr(firstSpacePos + 1);
    std::vector<std::string> argumentList;
    std::string currentArgument;
    for( auto& c : argumentSubstring)
    {
        if(c == ' ')
        {
           argumentList.push_back(currentArgument) ;
           currentArgument = "";
        }
        else
        {
            currentArgument += c ;
        }
    }
    argumentList.push_back(currentArgument) ;
    parsedResult.type = command ;
    parsedResult.args = argumentList ;

    return parsedResult;
 }
