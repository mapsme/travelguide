TEMPLATE = app
TARGET = env_tests
CONFIG += console warn_on
CONFIG -= app_bundle

INCLUDEPATH += ../3rdparty/boost ../3rdparty/googletest/include

HEADERS += \
  assert.hpp \
  condition.hpp \
  exception.hpp \
  file_handle.hpp \
  file_system.hpp \
  logging.hpp \
  message_list.hpp \
  mutex.hpp \
  posix.hpp \
  source_address.hpp \
  strings.hpp \
  writer.hpp \
  thread.hpp \
  message_std.hpp \
  reader.hpp \
  latlon.hpp \

SOURCES += \
  assert.cpp \
  condition_posix.cpp \
  file_handle.cpp \
  file_system.cpp \
  logging.cpp \
  mutex_posix.cpp \
  posix.cpp \
  source_address.cpp \
  thread_posix.cpp \
  strings.cpp \

# utf8proc
SOURCES += \
  ../3rdparty/utf8proc/utf8proc.c \

# unit tests
SOURCES += \
  ../3rdparty/googletest/src/gtest-all.cc \
  ../3rdparty/googletest/src/gtest_main.cc \
  tests/env_tests.cpp \
