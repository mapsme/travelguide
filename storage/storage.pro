TEMPLATE = app
TARGET = storage_tests
CONFIG += console warn_on
CONFIG -= app_bundle

INCLUDEPATH += ../3rdparty/boost ../3rdparty/googletest/include

HEADERS += \
  article_info.hpp \
  distance.hpp \
  storage.hpp \
  storage_builder.hpp \

SOURCES += \
  article_info.cpp \
  distance.cpp \
  storage.cpp \
  storage_builder.cpp \

# env sources
SOURCES += \
  ../env/logging.cpp \
  ../env/source_address.cpp \
  ../env/strings.cpp \
  ../env/assert.cpp \
  ../env/file_handle.cpp \
  ../env/posix.cpp \
  ../env/file_system.cpp \

# utf8proc
SOURCES += \
  ../3rdparty/utf8proc/utf8proc.c \

# unit tests
SOURCES += \
  ../3rdparty/googletest/src/gtest-all.cc \
  ../3rdparty/googletest/src/gtest_main.cc \
  tests/storage_test.cpp \
