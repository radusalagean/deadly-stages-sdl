TARGET = DeadlyStagesDemo
OBJS = main.o

CFLAGS = 
CXXFLAGS = $(CFLAGS) -std=c++14 -fno-rtti
ASFLAGS = $(CFLAGS)

# PSP Stuff
BUILD_PRX = 1
PSP_FW_VERSION = 150
PSP_LARGE_MEMORY = 1

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = DeadlyStagesDemo
# PSP_EBOOT_ICON = ICON0.PNG

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak