TEMPLATE = app
TARGET = genindex
CONFIG += console warn_on
CONFIG -= app_bundle qt

INCLUDEPATH += ../../3rdparty/boost

include (../../defines.pri)

SOURCES += \
  main.cpp \

# storage sources
SOURCES += \
  ../../storage/storage_builder.cpp \
  ../../storage/storage.cpp \
  ../../storage/article_info.cpp \
  ../../storage/distance.cpp \

# env sources
SOURCES += \
  ../../env/file_handle.cpp \
  ../../env/assert.cpp \
  ../../env/source_address.cpp \
  ../../env/logging.cpp \
  ../../env/posix.cpp \
  ../../env/strings.cpp \

# utf8proc
SOURCES += \
  ../../3rdparty/utf8proc/utf8proc.c \
