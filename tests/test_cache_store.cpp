#include <catch2/catch_test_macros.hpp>
#include "cache_store.h"
#include <thread>
#include <chrono>



TEST_CASE("Cache Store Tests", "[cache-store]") {
    SECTION("Set cache and get values") {
        CacheStore cacheStoreObj ;
        cacheStoreObj.set("Name" , "Vivek" , 60);
        auto result = cacheStoreObj.get("Name");
        REQUIRE(result == "Vivek");
    }
    SECTION("GET cache with missing key") {
        CacheStore cacheStoreObj ;
        auto result = cacheStoreObj.get("Name");
        REQUIRE(result.has_value() == false); // optional string should have no value 
    }

    SECTION("Delete a key ") {
        CacheStore cacheStoreObj ;
        cacheStoreObj.set("Name" , "Vivek" , 60);
        auto result = cacheStoreObj.get("Name");
        REQUIRE(result == "Vivek");
        cacheStoreObj.del("Name");
        result = cacheStoreObj.get("Name");
        REQUIRE(result.has_value() == false);
    }

    SECTION("Check if key exits") {
        CacheStore cacheStoreObj ;
        cacheStoreObj.set("Name" , "Vivek" , 60);
        auto result = cacheStoreObj.exists("Name");
        REQUIRE(result == true);
    }

     SECTION("Test expiry after TTL") {
        CacheStore cacheStoreObj ;
        cacheStoreObj.set("Name" , "Vivek" , 3);
        auto result = cacheStoreObj.get("Name");
        REQUIRE(result == "Vivek");
        std::this_thread::sleep_for(std::chrono::seconds(5));
        result = cacheStoreObj.get("Name");
        REQUIRE(result.has_value() == false);
    }

    SECTION("Test flush") {
        CacheStore cacheStoreObj ;
        cacheStoreObj.set("Name" , "Vivek" , 3);
        cacheStoreObj.set("Height" , "173" , 3);
        auto result = cacheStoreObj.keys();
        REQUIRE(result.size() == 2);
        cacheStoreObj.flush();
        result = cacheStoreObj.keys();
        REQUIRE(result.size() == 0);
        
    }
}
    