MKDIR?=mkdir -p
ProjDir := $(shell pwd)
KVERSION = $(shell uname -r)
MOD_BUILD_DIR ?= /lib/modules/$(KVERSION)/build

default: all

SRCS += $(wildcard *.c)
obj-m := $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(SRCS))))

all: module

module:
	$(MAKE) -C $(MOD_BUILD_DIR) SUBDIRS=$(ProjDir) modules

# directories: ${MOD_BUILD_DIR}
# ${MOD_BUILD_DIR}:
# 	-$(MKDIR) ${MOD_BUILD_DIR}

clean:
	$(MAKE) -C $(MOD_BUILD_DIR) SUBDIRS=$(ProjDir) clean

.PHONY: clean all

-include $(OBJS:%.o=%.d)