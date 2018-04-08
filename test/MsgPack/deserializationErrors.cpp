// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

static void check(const char* input, MsgPackError expected) {
  DynamicJsonVariant variant;

  MsgPackError error = deserializeMsgPack(variant, input);

  REQUIRE(error == expected);
}

TEST_CASE("Errors returned by deserializeMsgPack()") {
  SECTION("unsupported") {
    check("\xc4", MsgPackError::NotSupported);  // bin 8
    check("\xc5", MsgPackError::NotSupported);  // bin 16
    check("\xc6", MsgPackError::NotSupported);  // bin 32
    check("\xc7", MsgPackError::NotSupported);  // ext 8
    check("\xc8", MsgPackError::NotSupported);  // ext 16
    check("\xc9", MsgPackError::NotSupported);  // ext 32
    check("\xd4", MsgPackError::NotSupported);  // fixext 1
    check("\xd5", MsgPackError::NotSupported);  // fixext 2
    check("\xd6", MsgPackError::NotSupported);  // fixext 4
    check("\xd7", MsgPackError::NotSupported);  // fixext 8
    check("\xd8", MsgPackError::NotSupported);  // fixext 16
  }

  SECTION("unsupported in array") {
    check("\x91\xc4", MsgPackError::NotSupported);
  }
}
