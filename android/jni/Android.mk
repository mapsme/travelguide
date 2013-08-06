LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

TARGET_PLATFORM := android-9
LOCAL_CFLAGS := -ffunction-sections -fdata-sections -Wno-psabi

LOCAL_MODULE := tguide

LOCAL_HEADER_FILES := \
	and_storage.hpp \
	
# Android native files
LOCAL_SRC_FILES := \
	and_storage.cpp \

# Env files
LOCAL_SRC_FILES += \
	../../env/assert.cpp \
	../../env/file_handle.cpp \
	../../env/logging.cpp \
	../../env/posix.cpp \
	../../env/source_address.cpp \
	../../env/strings.cpp \
	../../3rdparty/utf8proc/utf8proc.c \

# Storage files
LOCAL_SRC_FILES += \
	../../storage/article_info.cpp \
	../../storage/distance.cpp \
	../../storage/storage.cpp \

include $(BUILD_SHARED_LIBRARY)
