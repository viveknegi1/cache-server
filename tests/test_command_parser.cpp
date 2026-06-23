#include <catch2/catch_test_macros.hpp>
#include "command_parser.h"


TEST_CASE("CommandParser parsing", "[command_parser]") {
    SECTION("SET with all arguments") {
        auto result = CommandParser::parse("SET name John 60\n");
        REQUIRE(result.type == CommandParser::CommandType::SET);
        REQUIRE(result.args.size() == 3);
        REQUIRE(result.args[0] == "name");
    }
    SECTION("GET with no arguments") {
        auto result = CommandParser::parse("GET\n");
        REQUIRE(result.type == CommandParser::CommandType::GET);
        REQUIRE(result.args.empty());
    }
     SECTION("SET with unicode characters") {
        auto result = CommandParser::parse("SET café latte\n");
        REQUIRE(result.type == CommandParser::CommandType::SET);
        REQUIRE(result.args.size() == 2);
        REQUIRE(result.args[0] == "café");
    }

    SECTION("GET with GET as an argument") {
        auto result = CommandParser::parse("GET GET Age\n");
        REQUIRE(result.type == CommandParser::CommandType::GET);
        REQUIRE(result.args.size() == 2);
    }

    SECTION("SET with long string an argument") {
        auto result = CommandParser::parse("SET " + std::string(10000, 'a') + " value\n");
        REQUIRE(result.type == CommandParser::CommandType::SET);
        REQUIRE(result.args.size() == 2);
    }

    SECTION("SET with negative TTL an argument") {
        auto result = CommandParser::parse("SET name -42\n");
        REQUIRE(result.type == CommandParser::CommandType::SET);
        REQUIRE(result.args.size() == 2);
    }

    SECTION("No command only whitespace") {
        auto result = CommandParser::parse("      \n");
        REQUIRE(result.type == CommandParser::CommandType::UNKNOWN);
        REQUIRE(result.args.size() == 0);
    }

    SECTION("Empty Command") {
        auto result = CommandParser::parse("\n");
        REQUIRE(result.type == CommandParser::CommandType::UNKNOWN);
        REQUIRE(result.args.size() == 0);
    }
}
