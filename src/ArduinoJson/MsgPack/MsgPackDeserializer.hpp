// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {

bool deserializeMsgPack(JsonVariant& variant, uint8_t* input) {
  uint8_t c = input[0];

  if ((c & 128) == 0) {
    variant = c & 127;
    return true;
  }

  switch (c) {
    case 0xc0:
      variant = static_cast<char*>(0);
      return true;

    case 0xc2:
      variant = false;
      return true;

    case 0xc3:
      variant = true;
      return true;

    default:
      return false;
  }
}
}  // namespace ArduinoJson
