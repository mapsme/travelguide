TEMPLATE = app
TARGET = genindex
CONFIG += console warn_on
CONFIG -= app_bundle

INCLUDEPATH += ../../3rdparty/boost

include (../../defines.pri)

SOURCES += \
  main.cpp \
  ../../storage/storage_builder.cpp \
  ../../storage/article_info.cpp \
  ../../storage/distance.cpp \
  ../../env/file_handle.cpp \
  ../../env/assert.cpp \
  ../../env/source_address.cpp \
  ../../env/logging.cpp \
  ../../env/posix.cpp \
  ../../env/strings.cpp \
  ../../3rdparty/utf8proc/utf8proc.c \
