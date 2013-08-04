TEMPLATE = app
TARGET = storage_tests
CONFIG += console warn_on
CONFIG -= app_bundle

INCLUDEPATH += ../3rdparty/boost ../3rdparty/googletest/include

HEADERS += \
  article.hpp \
  article_storage.hpp \

SOURCES += \
  article_storage.cpp

# unit tests
SOURCES += \
  ../3rdparty/googletest/src/gtest-all.cc \
  ../3rdparty/googletest/src/gtest_main.cc \
  tests/article_storage_test.cpp \
