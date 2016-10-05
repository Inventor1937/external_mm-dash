#/******************************************************************************
#*@file Android.mk
#*brief Rules for copiling the source files
#*******************************************************************************/

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := eng

LOCAL_SRC_FILES := $(call all-subdir-java-files)

LOCAL_MODULE := qcmediaplayer
LOCAL_MODULE_PATH := $(TARGET_OUT_JAVA_LIBRARIES)

ifeq ($(TARGET_ENABLE_QC_AV_ENHANCEMENTS),true)
$(info Qualcomm Audio/Video Enhancements are ENABLED. DASH & QCMediaPlayer are included.)
include $(BUILD_JAVA_LIBRARY)
else
$(info No Qualcomm Audio/Video Enhancements support)
endif
