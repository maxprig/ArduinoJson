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

  SECTION("nil") {
    const char* nil = 0;  // ArduinoJson uses a string for null
    check("\xc0", nil);
  }

  SECTION("bool") {
    check("\xc2", false);
    check("\xc3", true);
  }

  SECTION("7-bit positive integer") {
    check("\x00", 0);
    check("\x7F", 127);
  }

  SECTION("5-bit negative integer") {
    check("\xe0", -32);
    check("\xff", -1);
  }

  SECTION("8-bit unsigned integer") {
    check("\xcc\x00", 0);
    check("\xcc\xff", 255);
  }

  SECTION("16-bit unsigned integer") {
    check("\xcd\x00\x00", 0);
    check("\xcd\xFF\xFF", 65535);
    check("\xcd\x30\x39", 12345);
  }

  SECTION("32-bit unsigned integer") {
    check("\xce\x00\x00\x00\x00", 0x00000000);
    check("\xce\xFF\xFF\xFF\xFF", 0xFFFFFFFF);
    check("\xce\x12\x34\x56\x78", 0x12345678);
  }

  SECTION("64-bit unsigned integer") {
#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
    check<uint64_t>("\xCF\x00\x00\x00\x00\x00\x00\x00\x00", 0);
    check<uint64_t>("\xCF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 0xFFFFFFFFFFFFFFFF);
    check<uint64_t>("\xCF\x12\x34\x56\x78\x9A\xBC\xDE\xF0", 0x123456789ABCDEF0);
#else
    check<uint32_t>("\xCF\x00\x00\x00\x00\x00\x00\x00\x00", 0);
    check<uint32_t>("\xCF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 0xFFFFFFFF);
    check<uint32_t>("\xCF\x12\x34\x56\x78\x9A\xBC\xDE\xF0", 0x9ABCDEF0);
#endif
  }

  SECTION("8-bit signed integer") {
    check("\xd0\x00", 0);
    check("\xd0\xff", -1);
  }

  SECTION("16-bit signed integer") {
    check("\xD1\x00\x00", 0);
    check("\xD1\xFF\xFF", -1);
    check("\xD1\xCF\xC7", -12345);
  }

  SECTION("32-bit signed integer") {
    check("\xD2\x00\x00\x00\x00", 0);
    check("\xD2\xFF\xFF\xFF\xFF", -1);
    check("\xD2\xB6\x69\xFD\x2E", -1234567890);
  }

  SECTION("64-bit signed integer") {
#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
    check<uint64_t>("\xD3\x00\x00\x00\x00\x00\x00\x00\x00", 0);
    check<uint64_t>("\xD3\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 0xFFFFFFFFFFFFFFFF);
    check<uint64_t>("\xD3\x12\x34\x56\x78\x9A\xBC\xDE\xF0", 0x123456789ABCDEF0);
#else
    check<uint32_t>("\xD3\x00\x00\x00\x00\x00\x00\x00\x00", 0);
    check<uint32_t>("\xD3\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 0xFFFFFFFF);
    check<uint32_t>("\xD3\x12\x34\x56\x78\x9A\xBC\xDE\xF0", 0x9ABCDEF0);
#endif
  }
}
