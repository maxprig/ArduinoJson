// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {

bool deserializeMsgPack(JsonVariant& variant, uint8_t*) {
  variant = static_cast<char*>(0);
  return true;
}
}  // namespace ArduinoJson
