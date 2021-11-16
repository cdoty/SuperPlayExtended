PATH_LIB = $(PLATFORM_DIR)/lib
PATH_USR = $(PLATFORM_DIR)/usr
PATH_TOOLCHAIN = $(PLATFORM_DIR)/toolchain
CROSS_COMPILE = $(PATH_TOOLCHAIN)/bin/armv7a-cros-linux-gnueabi-
SYSROOT	= $(PLATFORM_DIR)/rootfs

LINK_OPTIONS = "-Wl,-rpath-link,$(SYSROOT)/lib -Wl,-rpath-link,$(SYSROOT)/usr/lib -Wl,-rpath-link,$(SYSROOT)/usr/lib/arm-linux-gnueabihf"	
LIBRARIES = -lc -lstdc++ -lpthread -lrt -ldl -lm -lshm -lGAL -lVSC -lOSAL -lPEAgent

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
LDFLAGS = 
