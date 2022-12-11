OBJ_DIR ?= ../obj
BIN_DIR ?= ../bin
PROJECT ?= drv_test
BACKSLASH?=/
OUTFILE?=$(BIN_DIR)/$(PROJECT)
MKDIR?=mkdir -p
convert=$(subst /,$(BACKSLASH),$1)
UNAME := $(shell uname)
ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif
CC=gcc
CXX=g++
CCFLAGS= -g
LDFLAGS = -pthread

default: all

.SUFFIXES: .bin .hex .srec

SRCS += $(wildcard *.c)
SRCS += $(wildcard *.cpp)
OBJS = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(SRCS))))
$(OBJ_DIR)/%.o: %.c
	$(CC) $(INCLUDES) $(CCFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) $(INCLUDES) $(CCFLAGS) -c $< -o $@
	
.PHONY: clean all

all: directories $(OUTFILE)

$(OUTFILE): $(OBJS)
	$(CC) $(LDFLAGS) -o $(OUTFILE) $(OBJS)

directories: ${OBJ_DIR} ${BIN_DIR}
${OBJ_DIR}:
	-$(MKDIR) ${OBJ_DIR}
${BIN_DIR}:
	-$(MKDIR) ${BIN_DIR}

clean:
	rm -rf $(OBJS)
	rm -rf $(OUTFILE)

-include $(OBJS:%.o=%.d)