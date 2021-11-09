PATH_SDK = $(PLATFORM_DIR)
PATH_LIB = $(PATH_SDK)/lib
PATH_USR = $(PATH_SDK)/usr
PATH_TOOLCHAIN = $(PATH_SDK)/toolchain
SYSROOT	= $(PATH_SDK)/rootfs

LINK_OPTIONS = "-Wl,-rpath-link,$(SYSROOT)/lib -Wl,-rpath-link,$(SYSROOT)/usr/lib -Wl,-rpath-link,$(SYSROOT)/usr/lib/arm-linux-gnueabihf" \
	-lc -lstdc++ -lpthread -lrt -ldl -lm -lshm -lGAL -lVSC -lOSAL -lPEAgent
CROSS_COMPILE = $(PATH_TOOLCHAIN)/bin/armv7a-cros-linux-gnueabi-
C = $(CROSS_COMPILE)gcc --sysroot=$(SYSROOT) $(LINK_OPTIONS)
CXX = $(CROSS_COMPILE)g++ --sysroot=$(SYSROOT)
AR = $(CROSS_COMPILE)ar
AS = $(CROSS_COMPILE)as
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy
STRIP = $(CROSS_COMPILE)strip
RANLIB = $(CROSS_COMPILE)ranlib

COMMON_FLAGS = -I$(SYSROOT)/usr/include $(INCLUDES) -DEGL_API_FB -DSTEAMLINK

CFLAGS = -std=c99 $(COMMON_FLAGS)
CXXFLAGS = -std=c++14 -fvisibility-inlines-hidden $(COMMON_FLAGS)
