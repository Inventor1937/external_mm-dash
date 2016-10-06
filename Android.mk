QCOM_MEDIA_ROOT := $(call my-dir)
ifneq ($(TARGET_DISABLE_DASH),true)
ifeq ($(TARGET_ENABLE_QC_MEDIAPLAYER),true)
include $(QCOM_MEDIA_ROOT)/jni/Android.mk
include $(QCOM_MEDIA_ROOT)/QCMediaPlayer/Android.mk
include $(QCOM_MEDIA_ROOT)/QCMediaPlayer/native/Android.mk
endif
include $(QCOM_MEDIA_ROOT)/dashplayer/Android.mk
endif
