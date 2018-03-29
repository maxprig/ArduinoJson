// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

template <typename Integer>
void check(uint8_t input[], Integer expected) {
  DynamicJsonVariant variant;

  bool success = deserializeMsgPack(variant, input);

  REQUIRE(success == true);
  REQUIRE(variant.is<Integer>());
  REQUIRE(variant.as<Integer>() == expected);
}

TEST_CASE("deserializeMsgPack(JsonVariant&)") {
  DynamicJsonVariant variant;

  SECTION("nil format") {
    uint8_t input[] = {0xc0};
    check<const char*>(input, 0);
  }

  SECTION("bool format family") {
    SECTION("false") {
      uint8_t input[] = {0xc2};
      check(input, false);
    }

    SECTION("true") {
      uint8_t input[] = {0xc3};
      check(input, true);
    }
  }

  SECTION("int format family") {
    SECTION("7-bit positive integer") {
      SECTION("0") {
        uint8_t input[] = {0x00};
        check(input, 0);
      }

      SECTION("127") {
        uint8_t input[] = {0x7F};
        check(input, 127);
      }
    }

    SECTION("5-bit negative integer") {
      SECTION("-32") {
        uint8_t input[] = {0xe0};
        check(input, -32);
      }

      SECTION("-1") {
        uint8_t input[] = {0xff};
        check(input, -1);
      }
    }

    SECTION("8-bit unsigned integer") {
      SECTION("0") {
        uint8_t input[] = {0xcc, 0x00};
        check(input, 0);
      }

      SECTION("255") {
        uint8_t input[] = {0xcc, 0xff};
        check(input, 255);
      }
    }

    SECTION("16-bit big-endian unsigned integer") {
      SECTION("0") {
        uint8_t input[] = {0xcd, 0x00, 0x00};
        check(input, 0);
      }

      SECTION("65535") {
        uint8_t input[] = {0xcd, 0xFF, 0xFF};
        check(input, 65535);
      }

      SECTION("12345") {
        uint8_t input[] = {0xcd, 0x30, 0x39};
        check(input, 12345);
      }
    }

    SECTION("32-bit big-endian unsigned integer") {
      SECTION("0x00000000") {
        uint8_t input[] = {0xce, 0x00, 0x00, 0x00, 0x00};
        check(input, 0x00000000);
      }

      SECTION("0xFFFFFFFF") {
        uint8_t input[] = {0xce, 0xFF, 0xFF, 0xFF, 0xFF};
        check(input, 0xFFFFFFFF);
      }

      SECTION("0x12345678") {
        uint8_t input[] = {0xce, 0x12, 0x34, 0x56, 0x78};
        check(input, 0x12345678);
      }
    }

    SECTION("64-bit big-endian unsigned integer") {
      SECTION("0x0000000000000000") {
        uint8_t input[] = {0xcf, 0x00, 0x00, 0x00, 0x00,
                           0x00, 0x00, 0x00, 0x00};
#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
        check<uint64_t>(input, 0);
#else
        check<uint32_t>(input, 0);
#endif
      }

      SECTION("0xFFFFFFFFFFFFFFFF") {
        uint8_t input[] = {0xcf, 0xFF, 0xFF, 0xFF, 0xFF,
                           0xFF, 0xFF, 0xFF, 0xFF};
#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
        check<uint64_t>(input, 0xFFFFFFFFFFFFFFFF);
#else
        check<uint32_t>(input, 0xFFFFFFFF);
#endif
      }

      SECTION("0x123456789ABCDEF0") {
        uint8_t input[] = {0xcf, 0x12, 0x34, 0x56, 0x78,
                           0x9A, 0xBC, 0xDE, 0xF0};

#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
        check<uint64_t>(input, 0x123456789ABCDEF0);
#else
        check<uint32_t>(input, 0x9ABCDEF0);
#endif
      }
    }

    SECTION("8-bit signed integer") {
      SECTION("0") {
        uint8_t input[] = {0xd0, 0x00};
        check(input, 0);
      }

      SECTION("-1") {
        uint8_t input[] = {0xd0, 0xff};
        check(input, -1);
      }
    }

    SECTION("16-bit big-endian signed integer") {
      SECTION("0") {
        uint8_t input[] = {0xd1, 0x00, 0x00};
        check(input, 0);
      }

      SECTION("-1") {
        uint8_t input[] = {0xd1, 0xFF, 0xFF};
        check(input, -1);
      }

      SECTION("-12345") {
        uint8_t input[] = {0xd1, 0xcf, 0xc7};
        check(input, -12345);
      }
    }
  }
}
