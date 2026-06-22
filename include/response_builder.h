#pragma once
#include <string>
#include <vector>

class ResponseBuilder{

private:
  

public:
    static std::string success();
    static std::string successWithValues(const std::string& successValue);
    static std::string notFound();
    static std::string error(const std::string& errorMessage);
    static std::string result(bool resultData);
    static std::string keys(const std::vector<std::string>& availableKeys);

};