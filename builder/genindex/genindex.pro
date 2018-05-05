TEMPLATE = app
TARGET = genindex
CONFIG += console
CONFIG -= app_bundle qt warn_on

INCLUDEPATH += ../../3rdparty/boost

include (../../defines.pri)

# To exclude specific warnings from -Wall, they should be defined AFTER -Wall.
# CONFIG -= warn_on above removes "automatic" -Wall from qmake spec and allows to correctly silence warnings below.
QMAKE_CXXFLAGS += -Wall -Wno-unused-parameter -Wno-unused-local-typedef

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
