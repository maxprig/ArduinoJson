// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

template <size_t Capacity>
static void check(const char* input, MsgPackError expected) {
  StaticJsonVariant<Capacity> variant;

  MsgPackError error = deserializeMsgPack(variant, input);

  REQUIRE(error == expected);
}

TEST_CASE("deserializeMsgPack(StaticJsonVariant&)") {
  SECTION("single values always fit") {
    check<0>("\xc0", MsgPackError::Ok);                  // nil
    check<0>("\xc2", MsgPackError::Ok);                  // false
    check<0>("\xc3", MsgPackError::Ok);                  // true
    check<0>("\xcc\x00", MsgPackError::Ok);              // uint 8
    check<0>("\xcd\x30\x39", MsgPackError::Ok);          // uint 16
    check<0>("\xCE\x12\x34\x56\x78", MsgPackError::Ok);  // uint 32
  }

  SECTION("fixstr") {
    check<0>("\xA0", MsgPackError::Ok);
    check<0>("\xA1H", MsgPackError::NoMemory);
    check<4>("\xA1H", MsgPackError::Ok);
    check<4>("\xA5Hello", MsgPackError::NoMemory);
  }

  SECTION("str 8") {
    check<0>("\xD9\x00", MsgPackError::Ok);
    check<0>("\xD9\x01H", MsgPackError::NoMemory);
    check<4>("\xD9\x01H", MsgPackError::Ok);
    check<4>("\xD9\x05Hello", MsgPackError::NoMemory);
  }

  SECTION("str 16") {
    check<0>("\xDA\x00\x00", MsgPackError::Ok);
    check<0>("\xDA\x00\x01H", MsgPackError::NoMemory);
    check<4>("\xDA\x00\x01H", MsgPackError::Ok);
    check<4>("\xDA\x00\x05Hello", MsgPackError::NoMemory);
  }

  SECTION("str 32") {
    check<0>("\xDB\x00\x00\x00\x00", MsgPackError::Ok);
    check<0>("\xDB\x00\x00\x00\x01H", MsgPackError::NoMemory);
    check<4>("\xDB\x00\x00\x00\x01H", MsgPackError::Ok);
    check<4>("\xDB\x00\x00\x00\x05Hello", MsgPackError::NoMemory);
  }
}
