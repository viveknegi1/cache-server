#pragma once

#include<string>
#include<vector>

class CommandParser{

public :
   enum class CommandType {
    SET,   // internally 0
    GET,   // internally 1
    DEL,    // internally 2
    EXISTS, // internally 3
    KEYS, // internally 4
    FLUSH, // internally 5
    SAVE, // internally 6
    UNKNOWN   // internally 7 
};
   
    struct ParsedCommand {
    CommandType type;
    std::vector<std::string> args;
    } ;

    // since this method won't touch any class members, make it statics so that we don't create object to invoke this.
    static ParsedCommand parse(const std::string& rawCommandString); 

};




