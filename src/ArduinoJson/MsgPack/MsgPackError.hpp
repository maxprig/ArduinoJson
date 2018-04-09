// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {

class MsgPackError {
 public:
  enum Code { Ok, NotSupported, NoMemory, NotAnArray, NotAnObject };

  MsgPackError() {}

  MsgPackError(Code code) : _code(code) {}

  operator bool() const {
    return _code != Ok;
  }

  friend bool operator==(const MsgPackError& err, Code code) {
    return err._code == code;
  }

  friend bool operator==(Code code, const MsgPackError& err) {
    return err._code == code;
  }

  friend bool operator!=(const MsgPackError& err, Code code) {
    return err._code != code;
  }

  friend bool operator!=(Code code, const MsgPackError& err) {
    return err._code != code;
  }

  const char* toString() const {
    switch (_code) {
      case Ok:
        return "Ok";
      case NotSupported:
        return "NotSupported";
      case NoMemory:
        return "NoMemory";
      case NotAnArray:
        return "NotAnArray";
      case NotAnObject:
        return "NotAnObject";
      default:
        return "???";
    }
  }

  friend std::ostream& operator<<(std::ostream& os, const MsgPackError& err) {
    os << err.toString();
    return os;
  }

 private:
  Code _code;
};

}  // namespace ArduinoJson
