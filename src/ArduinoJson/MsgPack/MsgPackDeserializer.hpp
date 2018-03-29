// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {

template <typename T, uint8_t size>
inline T readInteger(const uint8_t*& input) {
  T value = *input++;
  for (uint8_t i = 1; i < size; i++) {
    value = static_cast<T>(value << 8);
    value = static_cast<T>(value | *input++);
  }
  return value;
}

inline bool deserializeMsgPack(JsonVariant& variant, const uint8_t* input) {
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
      variant = readInteger<uint8_t, 1>(input);
      return true;

    case 0xcd:
      variant = readInteger<uint16_t, 2>(input);
      return true;

    case 0xce:
      variant = readInteger<uint32_t, 4>(input);
      return true;

    case 0xcf:
#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
      variant = readInteger<uint64_t, 8>(input);
#else
      variant = readInteger<uint32_t, 8>(input);
#endif
      return true;

    case 0xd0:
      variant = readInteger<int8_t, 1>(input);
      return true;

    case 0xd1:
      variant = readInteger<int16_t, 2>(input);
      return true;

    case 0xd2:
      variant = readInteger<int32_t, 4>(input);
      return true;

    default:
      return false;
  }
}

inline bool deserializeMsgPack(JsonVariant& variant, const char* input) {
  return deserializeMsgPack(variant, reinterpret_cast<const uint8_t*>(input));
}
}  // namespace ArduinoJson
