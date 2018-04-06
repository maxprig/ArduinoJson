// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("deserializeMsgPack(JsonObject&)") {
  DynamicJsonObject object;

  SECTION("fixmap") {
    SECTION("empty") {
      const char* input = "\x80";

      bool success = deserializeMsgPack(object, input);

      REQUIRE(success == true);
      REQUIRE(object.size() == 0);
    }

    SECTION("two integers") {
      const char* input = "\x82\xA3one\x01\xA3two\x02";

      bool success = deserializeMsgPack(object, input);

      REQUIRE(success == true);
      REQUIRE(object.size() == 2);
      REQUIRE(object["one"] == 1);
      REQUIRE(object["two"] == 2);
    }
  }

  SECTION("map 16") {
    SECTION("empty") {
      const char* input = "\xDE\x00\x00";

      bool success = deserializeMsgPack(object, input);

      REQUIRE(success == true);
      REQUIRE(object.size() == 0);
    }

    SECTION("two strings") {
      const char* input = "\xDE\x00\x02\xA1H\xA5hello\xA1W\xA5world";

      bool success = deserializeMsgPack(object, input);

      REQUIRE(success == true);
      REQUIRE(object.size() == 2);
      REQUIRE(object["H"] == "hello");
      REQUIRE(object["W"] == "world");
    }
  }

  SECTION("map 32") {
    SECTION("empty") {
      const char* input = "\xDF\x00\x00\x00\x00";

      bool success = deserializeMsgPack(object, input);

      REQUIRE(success == true);
      REQUIRE(object.size() == 0);
    }

    SECTION("two floats") {
      const char* input =
          "\xDF\x00\x00\x00\x02\xA4zero\xCA\x00\x00\x00\x00\xA2pi\xCA\x40\x48"
          "\xF5\xC3";

      bool success = deserializeMsgPack(object, input);

      REQUIRE(success == true);
      REQUIRE(object.size() == 2);
      REQUIRE(object["zero"] == 0.0f);
      REQUIRE(object["pi"] == 3.14f);
    }
  }
}
