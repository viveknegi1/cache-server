#include "response_builder.h"

std::string ResponseBuilder::success()
{
    return "OK\n";
}

 std::string ResponseBuilder::successWithValues(const std::string& successValue)
{
    return "OK " + successValue + "\n";
}

 std::string ResponseBuilder::notFound()
{
    return "NOT_FOUND\n";
}

 std::string ResponseBuilder::error(const std::string& errorMessage)
{
    return "ERROR " + errorMessage + "\n";
}

 std::string ResponseBuilder::result(bool resultData)
{
    std::string result = resultData ? "1\n" : "0\n" ;
    return result ;
}

std::string ResponseBuilder::keys(const std::vector<std::string>& availableKeys)
{
    std::string keys;
    for(const auto& key : availableKeys)
    {
        keys += key + "\n" ;
    }
    keys += "END\n";
    return keys;
}

