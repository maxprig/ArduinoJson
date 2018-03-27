// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {

inline bool deserializeMsgPack(JsonVariant& variant, uint8_t* input) {
  uint8_t c = *input++;

  if ((c & 0x80) == 0) {
    variant = c;
    return true;
  }

  if ((c & 0xe0) == 0xe0) {
    variant = static_cast<int8_t>(c);
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

    case 0xcc:
      variant = *input++;
      return true;

    case 0xcd: {
      uint8_t byte1 = *input++;
      uint8_t byte2 = *input++;
      variant = (byte1 << 8) | byte2;
      return true;
    }

    case 0xce: {
      uint32_t value = *input++;
      for (uint8_t i = 1; i < 4; i++) {
        value <<= 8;
        value |= *input++;
      }
      variant = value;
      return true;
    }

    case 0xcf: {
#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
      uint64_t value = *input++;
#else
      uint32_t value = *input++;
#endif
      for (uint8_t i = 1; i < 8; i++) {
        value <<= 8;
        value |= *input++;
      }
      variant = value;
      return true;
    }

    default:
      return false;
  }
}
}  // namespace ArduinoJson
