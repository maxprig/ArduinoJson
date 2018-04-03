// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Deserialization/StringWriter.hpp"
#include "../JsonVariant.hpp"
#include "../Memory/JsonBuffer.hpp"
#include "../TypeTraits/IsConst.hpp"

namespace ArduinoJson {
namespace Internals {

// Parse JSON string to create JsonArrays and JsonObjects
// This internal class is not indended to be used directly.
// Instead, use JsonBuffer.parseArray() or .parseObject()
template <typename TReader, typename TWriter>
class MsgPackDeserializer {
 public:
  MsgPackDeserializer(JsonBuffer *buffer, TReader reader, TWriter writer,
                      uint8_t nestingLimit)
      : _buffer(buffer),
        _reader(reader),
        _writer(writer),
        _nestingLimit(nestingLimit) {}
  bool parse(JsonArray &destination);
  bool parse(JsonObject &destination);
  bool parse(JsonVariant &variant) {
    using namespace Internals;
    uint8_t c = readOne();

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
        variant = static_cast<char *>(0);
        return true;

      case 0xc2:
        variant = false;
        return true;

      case 0xc3:
        variant = true;
        return true;

      case 0xcc:
        variant = readInteger<uint8_t, 1>();
        return true;

      case 0xcd:
        variant = readInteger<uint16_t, 2>();
        return true;

      case 0xce:
        variant = readInteger<uint32_t, 4>();
        return true;

      case 0xcf:
#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
        variant = readInteger<uint64_t, 8>();
#else
        variant = readInteger<uint32_t, 8>();
#endif
        return true;

      case 0xd0:
        variant = readInteger<int8_t, 1>();
        return true;

      case 0xd1:
        variant = readInteger<int16_t, 2>();
        return true;

      case 0xd2:
        variant = readInteger<int32_t, 4>();
        return true;

      case 0xd3:
#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
        variant = readInteger<int64_t, 8>();
#else
        variant = readInteger<int32_t, 8>();
#endif
        return true;

      case 0xca:
        variant = readFloat<float>();
        return true;

      case 0xcb:
        variant = readDouble<double>();
        return true;

      default:
        return false;
    }
  }

 private:
  uint8_t readOne() {
    char c = _reader.current();
    _reader.move();
    return c;
  }

  template <typename T, uint8_t size>
  T readInteger() {
    T value = static_cast<T>(readOne());
    for (uint8_t i = 1; i < size; i++) {
      value = static_cast<T>(value << 8);
      value = static_cast<T>(value | readOne());
    }
    return value;
  }

  template <typename T>
  typename EnableIf<sizeof(T) == 4, T>::type readFloat() {
    T value;
    uint8_t *p = reinterpret_cast<uint8_t *>(&value);
#if ARDUINOJSON_USE_LITTLE_ENDIAN_FLOAT
    p[3] = readOne();
    p[2] = readOne();
    p[1] = readOne();
    p[0] = readOne();
#else
    p[0] = readOne();
    p[1] = readOne();
    p[2] = readOne();
    p[3] = readOne();
#endif
    return value;
  }
  template <typename T>
  typename EnableIf<sizeof(T) == 8, T>::type readDouble() {
    T value;
    uint8_t *p = reinterpret_cast<uint8_t *>(&value);
    for (uint8_t i = 0; i < 8; i++)
#if ARDUINOJSON_USE_LITTLE_ENDIAN_FLOAT
      p[7 - i] = readOne();
#else
      p[i] = readOne();
#endif
    return value;
  }

  template <typename T>
  typename EnableIf<sizeof(T) == 4, T>::type readDouble() {
    T value;
    uint8_t *p = reinterpret_cast<uint8_t *>(&value);
#if ARDUINOJSON_USE_LITTLE_ENDIAN_FLOAT
    uint8_t o[] = {3, 2, 1, 0};
#else
    uint8_t o[] = {0, 1, 2, 3};
#endif
    uint8_t a = readOne();
    uint8_t b = readOne();
    uint8_t c = readOne();
    uint8_t d = readOne();
    uint8_t e = readOne();
    _reader.move();
    _reader.move();
    _reader.move();
    p[o[0]] = uint8_t((a & 0xC0) | (a << 3 & 0x3f) | (b >> 5));
    p[o[1]] = uint8_t((b << 3) | (c >> 5));
    p[o[2]] = uint8_t((c << 3) | (d >> 5));
    p[o[3]] = uint8_t((d << 3) | (e >> 5));

    return value;
  }

  JsonBuffer *_buffer;
  TReader _reader;
  TWriter _writer;
  uint8_t _nestingLimit;
};

template <typename TJsonBuffer, typename TString, typename Enable = void>
struct MsgPackDeserializerBuilder {
  typedef typename StringTraits<TString>::Reader InputReader;
  typedef MsgPackDeserializer<InputReader, TJsonBuffer &> TParser;

  static TParser makeMsgPackDeserializer(TJsonBuffer *buffer, TString &json,
                                         uint8_t nestingLimit) {
    return TParser(buffer, InputReader(json), *buffer, nestingLimit);
  }
};

template <typename TJsonBuffer, typename TChar>
struct MsgPackDeserializerBuilder<
    TJsonBuffer, TChar *, typename EnableIf<!IsConst<TChar>::value>::type> {
  typedef typename StringTraits<TChar *>::Reader TReader;
  typedef StringWriter<TChar> TWriter;
  typedef MsgPackDeserializer<TReader, TWriter> TParser;

  static TParser makeMsgPackDeserializer(TJsonBuffer *buffer, TChar *json,
                                         uint8_t nestingLimit) {
    return TParser(buffer, TReader(json), TWriter(json), nestingLimit);
  }
};

template <typename TJsonBuffer, typename TString>
inline typename MsgPackDeserializerBuilder<TJsonBuffer, TString>::TParser
makeMsgPackDeserializer(TJsonBuffer *buffer, TString &json,
                        uint8_t nestingLimit) {
  return MsgPackDeserializerBuilder<
      TJsonBuffer, TString>::makeMsgPackDeserializer(buffer, json,
                                                     nestingLimit);
}
}  // namespace Internals
}  // namespace ArduinoJson
