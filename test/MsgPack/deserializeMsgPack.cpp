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
    REQUIRE(variant.as<char*>() == static_cast<const char*>(0));
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

    SECTION("16-bit big-endian unsigned integer") {
      SECTION("0") {
        uint8_t input[] = {0xcd, 0x00, 0x00};

        bool success = deserializeMsgPack(variant, input);

        REQUIRE(success == true);
        REQUIRE(variant.is<int>());
        REQUIRE(variant.as<int>() == 0);
      }

      SECTION("65535") {
        uint8_t input[] = {0xcd, 0xFF, 0xFF};

        bool success = deserializeMsgPack(variant, input);

        REQUIRE(success == true);
        REQUIRE(variant.is<int>());
        REQUIRE(variant.as<int>() == 65535);
      }

      SECTION("12345") {
        uint8_t input[] = {0xcd, 0x30, 0x39};

        bool success = deserializeMsgPack(variant, input);

        REQUIRE(success == true);
        REQUIRE(variant.is<int>());
        REQUIRE(variant.as<int>() == 12345);
      }
    }

    SECTION("32-bit big-endian unsigned integer") {
      SECTION("0x00000000") {
        uint8_t input[] = {0xce, 0x00, 0x00, 0x00, 0x00};

        bool success = deserializeMsgPack(variant, input);

        REQUIRE(success == true);
        REQUIRE(variant.is<uint32_t>());
        REQUIRE(variant.as<uint32_t>() == 0x00000000);
      }

      SECTION("0xFFFFFFFF") {
        uint8_t input[] = {0xce, 0xFF, 0xFF, 0xFF, 0xFF};

        bool success = deserializeMsgPack(variant, input);

        REQUIRE(success == true);
        REQUIRE(variant.is<uint32_t>());
        REQUIRE(variant.as<uint32_t>() == 0xFFFFFFFF);
      }

      SECTION("0x12345678") {
        uint8_t input[] = {0xce, 0x12, 0x34, 0x56, 0x78};

        bool success = deserializeMsgPack(variant, input);

        REQUIRE(success == true);
        REQUIRE(variant.is<uint32_t>());
        REQUIRE(variant.as<uint32_t>() == 0x12345678);
      }
    }

    SECTION("64-bit big-endian unsigned integer") {
      SECTION("0x0000000000000000") {
        uint8_t input[] = {0xcf, 0x00, 0x00, 0x00, 0x00,
                           0x00, 0x00, 0x00, 0x00};

        bool success = deserializeMsgPack(variant, input);

        REQUIRE(success == true);

#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
        REQUIRE(variant.is<uint64_t>());
        REQUIRE(variant.as<uint64_t>() == 0);
#else
        REQUIRE(variant.is<uint32_t>());
        REQUIRE(variant.as<uint32_t>() == 0);
#endif
      }

      SECTION("0xFFFFFFFFFFFFFFFF") {
        uint8_t input[] = {0xcf, 0xFF, 0xFF, 0xFF, 0xFF,
                           0xFF, 0xFF, 0xFF, 0xFF};

        bool success = deserializeMsgPack(variant, input);

        REQUIRE(success == true);

#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
        REQUIRE(variant.is<uint64_t>());
        REQUIRE(variant.as<uint64_t>() == 0xFFFFFFFFFFFFFFFF);
#else
        REQUIRE(variant.is<uint32_t>());
        REQUIRE(variant.as<uint32_t>() == 0xFFFFFFFF);
#endif
      }

      SECTION("0x123456789ABCDEF0") {
        uint8_t input[] = {0xcf, 0x12, 0x34, 0x56, 0x78,
                           0x9A, 0xBC, 0xDE, 0xF0};

        bool success = deserializeMsgPack(variant, input);

        REQUIRE(success == true);

#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
        REQUIRE(variant.is<uint64_t>());
        REQUIRE(variant.as<uint64_t>() == 0x123456789ABCDEF0);
#else
        REQUIRE(variant.is<uint32_t>());
        REQUIRE(variant.as<uint32_t>() == 0x9ABCDEF0);
#endif
      }
    }

    SECTION("8-bit signed integer") {
      SECTION("0") {
        uint8_t input[] = {0xd0, 0x00};

        bool success = deserializeMsgPack(variant, input);

        REQUIRE(success == true);
        REQUIRE(variant.is<int>());
        REQUIRE(variant.as<int>() == 0);
      }

      SECTION("-1") {
        uint8_t input[] = {0xd0, 0xff};

        bool success = deserializeMsgPack(variant, input);

        REQUIRE(success == true);
        REQUIRE(variant.is<int>());
        REQUIRE(variant.as<int>() == -1);
      }
    }
  }
}
