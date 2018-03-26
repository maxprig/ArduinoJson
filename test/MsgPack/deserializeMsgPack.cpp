// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("deserializeMsgPack(JsonVariant&)") {
  DynamicJsonVariant variant;

  SECTION("nil format") {
    uint8_t input[] = {0xc0};

    bool success = deserializeMsgPack(variant, input);

    REQUIRE(success == true);
    REQUIRE(variant.is<char*>());
    REQUIRE(variant.as<char*>() == NULL);
  }

  SECTION("bool format family") {
    SECTION("false") {
      uint8_t input[] = {0xc2};

      bool success = deserializeMsgPack(variant, input);

      REQUIRE(success == true);
      REQUIRE(variant.is<bool>());
      REQUIRE(variant.as<bool>() == false);
    }

    SECTION("true") {
      uint8_t input[] = {0xc3};

      bool success = deserializeMsgPack(variant, input);

      REQUIRE(success == true);
      REQUIRE(variant.is<bool>());
      REQUIRE(variant.as<bool>() == true);
    }
  }

  SECTION("int format family") {
    SECTION("7-bit positive integer") {
      SECTION("0") {
        uint8_t input[] = {0x00};

        bool success = deserializeMsgPack(variant, input);

        REQUIRE(success == true);
        REQUIRE(variant.is<int>());
        REQUIRE(variant.as<int>() == 0);
      }

      SECTION("127") {
        uint8_t input[] = {0x7F};

        bool success = deserializeMsgPack(variant, input);

        REQUIRE(success == true);
        REQUIRE(variant.is<int>());
        REQUIRE(variant.as<int>() == 127);
      }
    }

    SECTION("5-bit negative integer") {
      SECTION("-32") {
        uint8_t input[] = {0xe0};

        bool success = deserializeMsgPack(variant, input);

        REQUIRE(success == true);
        REQUIRE(variant.is<int>());
        REQUIRE(variant.as<int>() == -32);
      }

      SECTION("-1") {
        uint8_t input[] = {0xff};

        bool success = deserializeMsgPack(variant, input);

        REQUIRE(success == true);
        REQUIRE(variant.is<int>());
        REQUIRE(variant.as<int>() == -1);
      }
    }

    SECTION("8-bit unsigned integer") {
      SECTION("0") {
        uint8_t input[] = {0xcc, 0x00};

        bool success = deserializeMsgPack(variant, input);

        REQUIRE(success == true);
        REQUIRE(variant.is<int>());
        REQUIRE(variant.as<int>() == 0);
      }

      SECTION("255") {
        uint8_t input[] = {0xcc, 0xff};

        bool success = deserializeMsgPack(variant, input);

        REQUIRE(success == true);
        REQUIRE(variant.is<int>());
        REQUIRE(variant.as<int>() == 255);
      }
    }
  }
}
