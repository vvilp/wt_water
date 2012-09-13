# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#  

#-Werror
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := JNIOpenGL
LOCAL_SRC_FILES := \
wt_native.c  wt_main.c  wt_gl_draw.c  \
../../../wt_core/wt_body.c \
../../../wt_core/wt_shape.c \
../../../wt_core/wt_world.c  \
../../../wt_common/wt_array.c \
../../../wt_core/wt_collision.c \
../../../wt_core/wt_pvf_fluid.c \
../../../wt_core/wt_spatial_hash.c \

LOCAL_CFLAGS    := -O -g -std=c99
LOCAL_LDLIBS    :=-L$(SYSROOT)/usr/lib -lGLESv2  
LOCAL_LDLIBS    +=-L$(SYSROOT)/usr/lib -lGLESv1_CM  
LOCAL_LDLIBS    += -L$(SYSROOT)/usr/lib -llog

include $(BUILD_SHARED_LIBRARY)
