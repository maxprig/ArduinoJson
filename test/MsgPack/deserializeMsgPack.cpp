// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

template <typename T>
void check(const char* input, T expected) {
  DynamicJsonVariant variant;

  bool success = deserializeMsgPack(variant, input);

  REQUIRE(success == true);
  REQUIRE(variant.is<T>());
  REQUIRE(variant.as<T>() == expected);
}

TEST_CASE("deserializeMsgPack(JsonVariant&)") {
  DynamicJsonVariant variant;

  SECTION("nil format") {
    check<const char*>("\xc0", 0);
  }

  SECTION("bool format family") {
    SECTION("false") {
      check("\xc2", false);
    }

    SECTION("true") {
      check("\xc3", true);
    }
  }

  SECTION("int format family") {
    SECTION("7-bit positive integer") {
      SECTION("0") {
        check("\x00", 0);
      }

      SECTION("127") {
        check("\x7F", 127);
      }
    }

    SECTION("5-bit negative integer") {
      SECTION("-32") {
        check("\xe0", -32);
      }

      SECTION("-1") {
        check("\xff", -1);
      }
    }

    SECTION("8-bit unsigned integer") {
      SECTION("0") {
        check("\xcc\x00", 0);
      }

      SECTION("255") {
        check("\xcc\xff", 255);
      }
    }

    SECTION("16-bit big-endian unsigned integer") {
      SECTION("0") {
        check("\xcd\x00\x00", 0);
      }

      SECTION("65535") {
        check("\xcd\xFF\xFF", 65535);
      }

      SECTION("12345") {
        check("\xcd\x30\x39", 12345);
      }
    }

    SECTION("32-bit big-endian unsigned integer") {
      SECTION("0x00000000") {
        check("\xce\x00\x00\x00\x00", 0x00000000);
      }

      SECTION("0xFFFFFFFF") {
        check("\xce\xFF\xFF\xFF\xFF", 0xFFFFFFFF);
      }

      SECTION("0x12345678") {
        check("\xce\x12\x34\x56\x78", 0x12345678);
      }
    }

    SECTION("64-bit big-endian unsigned integer") {
      SECTION("0x0000000000000000") {
        char input[] = "\xcf\x00\x00\x00\x00\x00\x00\x00\x00";
#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
        check<uint64_t>(input, 0);
#else
        check<uint32_t>(input, 0);
#endif
      }

      SECTION("0xFFFFFFFFFFFFFFFF") {
        char input[] = "\xCF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
        check<uint64_t>(input, 0xFFFFFFFFFFFFFFFF);
#else
        check<uint32_t>(input, 0xFFFFFFFF);
#endif
      }

      SECTION("0x123456789ABCDEF0") {
        char input[] = "\xcf\x12\x34\x56\x78\x9A\xBC\xDE\xF0";

#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
        check<uint64_t>(input, 0x123456789ABCDEF0);
#else
        check<uint32_t>(input, 0x9ABCDEF0);
#endif
      }
    }

    SECTION("8-bit signed integer") {
      SECTION("0") {
        check("\xd0\x00", 0);
      }

      SECTION("-1") {
        check("\xd0\xff", -1);
      }
    }

    SECTION("16-bit big-endian signed integer") {
      SECTION("0") {
        check("\xd1\x00\x00", 0);
      }

      SECTION("-1") {
        check("\xd1\xFF\xFF", -1);
      }

      SECTION("-12345") {
        check("\xd1\xcf\xc7", -12345);
      }
    }
  }
}
