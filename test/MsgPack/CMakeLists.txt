# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2018
# MIT License

add_executable(MsgPackTests
	deserializeMsgPack
)

target_link_libraries(MsgPackTests catch)
add_test(MsgPack MsgPackTests)
