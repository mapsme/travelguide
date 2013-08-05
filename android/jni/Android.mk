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

# Storage files
LOCAL_SRC_FILES += \
	../../storage/storage.cpp \
	../../storage/article_info_storage.cpp \
	../../storage/index_storage.cpp \

include $(BUILD_SHARED_LIBRARY)
