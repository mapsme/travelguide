TEMPLATE = app
TARGET = storage_tests
CONFIG += console warn_on
CONFIG -= app_bundle

INCLUDEPATH += ../3rdparty/boost ../3rdparty/googletest/include

HEADERS += \
  storage.hpp \
  article_info.hpp \
  storage_common.hpp \

SOURCES += \
  storage.cpp \
  article_info.cpp \

# env sources
SOURCES += \
  ../env/logging.cpp \
  ../env/source_address.cpp \

# unit tests
SOURCES += \
  ../3rdparty/googletest/src/gtest-all.cc \
  ../3rdparty/googletest/src/gtest_main.cc \
  tests/storage_test.cpp \
