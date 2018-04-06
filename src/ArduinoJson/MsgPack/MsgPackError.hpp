// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {

class MsgPackError {
 public:
  enum Code { Ok, Error };

  MsgPackError(Code code) : _code(code) {}

  MsgPackError(bool e) : _code(e ? Ok : Error) {}

  operator bool() const {
    return _code == Ok;
  }

  friend bool operator==(const MsgPackError& lhs, const MsgPackError& rhs) {
    return lhs._code == rhs._code;
  }

 private:
  Code _code;
};

}  // namespace ArduinoJson
