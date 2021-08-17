TOP := $(shell pwd)
BUILD := $(TOP)/build-$(shell $(CC) -dumpmachine)
BUILD := build-$(shell $(CC) -dumpmachine)

TARGET = vm

LIB_DTIME = ../lib_dtime

INCLUDE = -Iinclude -I../include
INCLUDE += -I../../lib_dtime

LDFLAGS = -lSDL -lpthread 

SOURCE := source

VPATH = ./include:../include:../../include:../lib_dtime

SRCS = $(wildcard $(SOURCE)/*.c)
SRCS += $(LIB_DTIME)/dtime.c

#OBJS = $(addprefix $(BUILD)/, $(notdir $(patsubst %.c, %.o, $(SRCS))))
OBJS = $(patsubst $(SOURCE)/%.c, $(BUILD)/%.o, $(SRCS))
DEPS = $(patsubst $(SOURCE)/%.c, $(BUILD)/%.d, $(SRCS))

all: $(BUILD) $(BUILD)/$(OBJS) target

include ./makefile.common


$(BUILD)/libdtime.o :  ../lib_dtime/dtime.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

#$(BUILD)/xcvm_eval.o : $(SOURCE)/xcvm_eval.c
#	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@
