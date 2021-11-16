LINK_OPTIONS =
LIBRARIES = -lc -lstdc++ -lpthread -lm -lopenmaxil -lbcm_host -lvcos -lvchiq_arm -lrt
C = gcc $(LINK_OPTIONS)
CXX = g++
AR = ar
AS = as
LD = ld
OBJCOPY = objcopy
STRIP = strip
RANLIB = ranlib

COMMON_FLAGS = $(INCLUDES) -mfpu=vfp -DHAVE_LIBBCM_HOST -DUSE_EXTERNAL_LIBBCM_HOST -DUSE_VCHIQ_ARM -DRASPBERRYPI

CFLAGS = -std=c99 $(COMMON_FLAGS)
CXXFLAGS = -std=c++14 -fvisibility-inlines-hidden $(COMMON_FLAGS)
LDFLAGS = -L/opt/vc/lib
