PATH_SDK = $(STEAMLINK_DIR)
PATH_LIB = $(PATH_SDK)/lib
PATH_USR = $(PATH_SDK)/usr
PATH_TOOLCHAIN = $(PATH_SDK)/toolchain
SYSROOT	= $(PATH_SDK)/rootfs

CROSS_COMPILE = $(PATH_TOOLCHAIN)/bin/armv7a-cros-linux-gnueabi-
CC = $(CROSS_COMPILE)gcc --sysroot=$(SYSROOT)
CXX = $(CROSS_COMPILE)g++ --sysroot=$(SYSROOT)
AR = $(CROSS_COMPILE)ar
AS = $(CROSS_COMPILE)as
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy
STRIP = $(CROSS_COMPILE)strip
RANLIB = $(CROSS_COMPILE)ranlib

COMMON_FLAGS = -I$(SYSROOT)/usr/include $(INCLUDES) -DSTEAMLINK

CFLAGS = -std=c99 $(COMMON_FLAGS)
CXXFLAGS = -std=c++14 -fvisibility-inlines-hidden $(COMMON_FLAGS)

LINK_OPTIONS = "-Wl,-rpath-link,$MARVELL_ROOTFS/lib -Wl,-rpath-link,$MARVELL_ROOTFS/usr/lib -Wl,-rpath-link,$MARVELL_ROOTFS/usr/lib/arm-linux-gnueabihf"
