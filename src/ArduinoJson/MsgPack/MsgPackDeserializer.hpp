// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {

namespace Internals {

template <typename T, uint8_t size>
inline T readInteger(const uint8_t*& input) {
  T value = static_cast<T>(*input++);
  for (uint8_t i = 1; i < size; i++) {
    value = static_cast<T>(value << 8);
    value = static_cast<T>(value | *input++);
  }
  return value;
}

template <typename T>
inline typename EnableIf<sizeof(T) == 4, T>::type readFloat(
    const uint8_t*& input) {
  T value;
  uint8_t* p = reinterpret_cast<uint8_t*>(&value);
#if ARDUINOJSON_USE_LITTLE_ENDIAN_FLOAT
  p[3] = *input++;
  p[2] = *input++;
  p[1] = *input++;
  p[0] = *input++;
#else
  p[0] = *input++;
  p[1] = *input++;
  p[2] = *input++;
  p[3] = *input++;
#endif
  return value;
}

template <typename T>
inline typename EnableIf<sizeof(T) == 8, T>::type readDouble(
    const uint8_t*& input) {
  T value;
  uint8_t* p = reinterpret_cast<uint8_t*>(&value);
  for (uint8_t i = 0; i < 8; i++)
#if ARDUINOJSON_USE_LITTLE_ENDIAN_FLOAT
    p[7 - i] = *input++;
#else
    p[i] = *input++;
#endif
  return value;
}
}  // namespace Internals

inline bool deserializeMsgPack(JsonVariant& variant, const uint8_t* input) {
  using namespace Internals;
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

    case 0xd3:
#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
      variant = readInteger<int64_t, 8>(input);
#else
      variant = readInteger<int32_t, 8>(input);
#endif
      return true;

    case 0xca:
      variant = readFloat<float>(input);
      return true;

    case 0xcb:
      variant = readDouble<double>(input);
      return true;

    default:
      return false;
  }
}

inline bool deserializeMsgPack(JsonVariant& variant, const char* input) {
  return deserializeMsgPack(variant, reinterpret_cast<const uint8_t*>(input));
}
}  // namespace ArduinoJson
