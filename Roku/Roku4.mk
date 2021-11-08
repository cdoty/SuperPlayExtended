# Copyright 2021 Roku, Inc.  All rights reserved.
PATH_SDK = $(ROKU_DIR)/platforms/Roku4
PATH_LIB = $(PATH_SDK)/lib
PATH_USR = $(PATH_SDK)/usr
PATH_TOOLCHAIN = $(PATH_SDK)/toolchain

CROSS_COMPILE = $(PATH_TOOLCHAIN)/bin/arm-roku-linux-gnueabi-
CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
AR = $(CROSS_COMPILE)ar
AS = $(CROSS_COMPILE)as
LD = $(CROSS_COMPILE)gcc
OBJCOPY = $(CROSS_COMPILE)objcopy
STRIP = $(CROSS_COMPILE)strip
RANLIB = $(CROSS_COMPILE)ranlib

COMMON_FLAGS = -mcpu=cortex-a9 -mthumb \
	-fvisibility=hidden -fdata-sections -ffunction-sections \
	-Wno-parentheses -U_FORTIFY_SOURCE \
	-I$(PATH_SDK)/include -I$(PATH_USR)/include $(INCLUDES) -DROKU

CFLAGS = -std=c99 $(COMMON_FLAGS)
CXXFLAGS = -std=c++14 -fvisibility-inlines-hidden $(COMMON_FLAGS)

PATH_LIBS = \
	-L $(PATH_LIB) -L$(PATH_USR)/lib

LDFLAGS = -Wl,--gc-sections -Wl,--copy-dt-needed-entries \
	-Wl,--allow-shlib-undefined -Wl,--no-as-needed \
	$(PATH_LIBS) -lc -lpthread -lrt -ldl -lstdc++

UTIL_MKSQUASHFS = ~/Roku/bin/mksquashfs_roku
UTIL_PAXCTL = ~/Roku/bin/paxctl
