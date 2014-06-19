LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := start_wfd
LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES := start_wfd.c 

include $(BUILD_EXECUTABLE)
