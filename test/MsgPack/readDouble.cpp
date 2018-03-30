// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace ArduinoJson::Internals;

template <typename T>
static void check(const char* input, T expected) {
  const uint8_t* bytes = reinterpret_cast<const uint8_t*>(input);
  REQUIRE(readDouble<T>(bytes) == expected);
}

TEST_CASE("Internals::readDouble()") {
  check("\x40\x09\x21\xCA\xC0\x83\x12\x6F", 3.1415f);
  check("\x00\x00\x00\x00\x00\x00\x00\x00", 0.0f);
  check("\x80\x00\x00\x00\x00\x00\x00\x00", -0.0f);
  check("\xC0\x5E\xDC\xCC\xCC\xCC\xCC\xCD", -123.45f);
}
