# Automatically generated makefile, created by the Introjucer
# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

ifeq ($(TARGET_ARCH_ABI), armeabi-v7a)
    LOCAL_ARM_MODE := arm
endif

LOCAL_MODULE := juce_jni
LOCAL_SRC_FILES := \
  ../../../Source/field_manager_tests.cpp\
  ../../../../common/field_manager.c\
  ../../../Source/Main.cpp\
  ../../../../../JUCE/modules/juce_core/juce_core.cpp\
  ../../../../../JUCE/modules/juce_data_structures/juce_data_structures.cpp\
  ../../../../../JUCE/modules/juce_events/juce_events.cpp\

ifeq ($(NDK_DEBUG),1)
  LOCAL_CPPFLAGS += -fsigned-char -fexceptions -frtti -g -I "../../JuceLibraryCode" -I "../../../../JUCE/modules" -O0 -std=c++11 -std=gnu++11 -D "JUCE_ANDROID=1" -D "JUCE_ANDROID_API_VERSION=10" -D "JUCE_ANDROID_ACTIVITY_CLASSNAME=com_yourcompany_field_manager_test_field_manager_test" -D JUCE_ANDROID_ACTIVITY_CLASSPATH=\"com/yourcompany/field_manager_test/field_manager_test\" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_ANDROID_7F0E4A25=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000"
  LOCAL_LDLIBS := -llog -lGLESv2
  LOCAL_CFLAGS += -fsigned-char -fexceptions -frtti -g -I "../../JuceLibraryCode" -I "../../../../JUCE/modules" -O0 -std=c++11 -std=gnu++11 -D "JUCE_ANDROID=1" -D "JUCE_ANDROID_API_VERSION=10" -D "JUCE_ANDROID_ACTIVITY_CLASSNAME=com_yourcompany_field_manager_test_field_manager_test" -D JUCE_ANDROID_ACTIVITY_CLASSPATH=\"com/yourcompany/field_manager_test/field_manager_test\" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_ANDROID_7F0E4A25=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000"
  LOCAL_LDLIBS := -llog -lGLESv2
else
  LOCAL_CPPFLAGS += -fsigned-char -fexceptions -frtti -I "../../JuceLibraryCode" -I "../../../../JUCE/modules" -O3 -std=c++11 -std=gnu++11 -D "JUCE_ANDROID=1" -D "JUCE_ANDROID_API_VERSION=10" -D "JUCE_ANDROID_ACTIVITY_CLASSNAME=com_yourcompany_field_manager_test_field_manager_test" -D JUCE_ANDROID_ACTIVITY_CLASSPATH=\"com/yourcompany/field_manager_test/field_manager_test\" -D "NDEBUG=1" -D "JUCER_ANDROID_7F0E4A25=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000"
  LOCAL_LDLIBS := -llog -lGLESv2
  LOCAL_CFLAGS += -fsigned-char -fexceptions -frtti -I "../../JuceLibraryCode" -I "../../../../JUCE/modules" -O3 -std=c++11 -std=gnu++11 -D "JUCE_ANDROID=1" -D "JUCE_ANDROID_API_VERSION=10" -D "JUCE_ANDROID_ACTIVITY_CLASSNAME=com_yourcompany_field_manager_test_field_manager_test" -D JUCE_ANDROID_ACTIVITY_CLASSPATH=\"com/yourcompany/field_manager_test/field_manager_test\" -D "NDEBUG=1" -D "JUCER_ANDROID_7F0E4A25=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000"
  LOCAL_LDLIBS := -llog -lGLESv2
endif

include $(BUILD_SHARED_LIBRARY)
