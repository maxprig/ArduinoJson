// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("deserializeMsgPack(JsonArray&)") {
  DynamicJsonArray array;

  SECTION("fixarray") {
    SECTION("empty") {
      const char* input = "\x90";

      bool success = deserializeMsgPack(array, input);

      REQUIRE(success == true);
      REQUIRE(array.size() == 0);
    }

    SECTION("two integers") {
      const char* input = "\x92\x01\x02";

      bool success = deserializeMsgPack(array, input);

      REQUIRE(success == true);
      REQUIRE(array.size() == 2);
      REQUIRE(array[0] == 1);
      REQUIRE(array[1] == 2);
    }
  }

  SECTION("array 16") {
    SECTION("empty") {
      const char* input = "\xDC\x00";

      bool success = deserializeMsgPack(array, input);

      REQUIRE(success == true);
      REQUIRE(array.size() == 0);
    }

    SECTION("two strings") {
      const char* input = "\xDC\x00\x02\xA5hello\xA5world";

      bool success = deserializeMsgPack(array, input);

      REQUIRE(success == true);
      REQUIRE(array.size() == 2);
      REQUIRE(array[0] == "hello");
      REQUIRE(array[1] == "world");
    }
  }
}
