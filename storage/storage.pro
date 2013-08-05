TEMPLATE = app
TARGET = storage_tests
CONFIG += console warn_on
CONFIG -= app_bundle

INCLUDEPATH += ../3rdparty/boost ../3rdparty/googletest/include

HEADERS += \
  storage.hpp \
  article_info_storage.hpp \
  article_info.hpp \
  index_storage.hpp \
  storage_common.hpp \

SOURCES += \
  article_info_storage.cpp \
  index_storage.cpp \
  storage.cpp \

# unit tests
SOURCES += \
  ../3rdparty/googletest/src/gtest-all.cc \
  ../3rdparty/googletest/src/gtest_main.cc \
  tests/storage_test.cpp \
