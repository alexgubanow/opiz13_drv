MKDIR?=mkdir -p
SYMLNK?=ln -s
# ProjDir := $(shell pwd)
# KVERSION = $(shell uname -r)
mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(patsubst %/,%,$(dir $(mkfile_path)))
BUILD_DIR ?= ${current_dir}/../bld
OUT_DIR ?= $(PWD)/../out

SRCS += $(wildcard *.c)
SRCS_SYMLINKS += $(wildcard *.c)
SRCS_SYMLINKS += $(wildcard *.h)
SRCS_SYMLINKS += Makefile
# #obj-m := $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(SRCS))))
obj-m += opiz13.o
opiz13-y := opiz13_main.o gpio.o pwr.o

default: module

module: ${SRCS_SYMLINKS}
	make -C /lib/modules/$(shell uname -r)/build M=${BUILD_DIR} modules

install:
	make -C /lib/modules/$(shell uname -r)/build M=${BUILD_DIR} modules_install
	depmod -A

${BUILD_DIR}:
	-$(MKDIR) ${BUILD_DIR}

${SRCS_SYMLINKS}: ${BUILD_DIR}
	[ -L ${BUILD_DIR}/$@ ] || ($(SYMLNK) ${current_dir}/$@ ${BUILD_DIR}/$@)

clean:
	make -C /lib/modules/$(shell uname -r)/build M=${BUILD_DIR} clean
	rm -rf ${BUILD_DIR}