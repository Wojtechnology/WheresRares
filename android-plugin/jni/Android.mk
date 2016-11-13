LOCAL_PATH := $(call my-dir)

CVROOT := /Users/ryancatoen/Downloads/OpenCV-android-sdk/sdk/native/jni
include $(CLEAR_VARS)
OPENCV_INSTALL_MODULES:=on
OPENCV_LIB_TYPE:=STATIC
include $(CVROOT)/OpenCV.mk

LOCAL_MODULE := libCVCode2

CODE_PATH := ../cv

LOCAL_SRC_FILES := $(CODE_PATH)/src/HumanTracking.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(CODE_PATH)/include

LOCAL_CFLAGS += -std=c++11 -frtti -fexceptions -fopenmp -w
include $(BUILD_SHARED_LIBRARY) 
