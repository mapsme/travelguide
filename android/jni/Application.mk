APP_STL := gnustl_static
APP_PLATFORM := android-9

LOCAL_PATH := $(call my-dir)
APP_CFLAGS += -I$(LOCAL_PATH)/../../3rdparty/boost

APP_GNUSTL_FORCE_CPP_FEATURES := exceptions rtti
