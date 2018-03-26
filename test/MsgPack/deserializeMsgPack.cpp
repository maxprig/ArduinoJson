// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("deserializeMsgPack(JsonVariant&)") {
  DynamicJsonVariant variant;

  SECTION("nil") {
    uint8_t input[] = {0xc0};

    bool success = deserializeMsgPack(variant, input);

    REQUIRE(success == true);
    REQUIRE(variant.is<char*>());
    REQUIRE(variant.as<char*>() == NULL);
  }

  SECTION("true") {
    uint8_t input[] = {0xc3};

    bool success = deserializeMsgPack(variant, input);

    REQUIRE(success == true);
    REQUIRE(variant.is<bool>());
    REQUIRE(variant.as<bool>() == true);
  }
}
