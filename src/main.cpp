#include "logger.h"
#include "config.h"
#include "command_parser.h"

int main()
{

Config::getInstance("config.ini");
Logger::getInstance().log(Logger::Level::INFO, "Cache Server Started");
return 0;

}