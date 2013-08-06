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
	../../env/strings.cpp \
	../../env/source_address.cpp \
	../../env/assert.cpp \
	../../3rdparty/utf8proc/utf8proc.c \


# Storage files
LOCAL_SRC_FILES += \
	../../storage/storage.cpp \
	../../storage/article_info.cpp \

include $(BUILD_SHARED_LIBRARY)
