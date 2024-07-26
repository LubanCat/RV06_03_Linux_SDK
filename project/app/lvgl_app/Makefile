# Makefile for calling CMake with specified toolchain file and install prefix
ifeq ($(APP_PARAM), )
	APP_PARAM:=../Makefile.param
	include $(APP_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)

PKG_INSTALL_TO_ROOTFS ?= YES
PKG_NAME := lvgl_app
PKG_BIN ?= out
PKG_BUILD ?= build

CURRENT_DIR := $(shell pwd)

# Set the path to the CMake toolchain file
ifeq ($(RK_ROOTFS_TYPE), ubuntu)
	TOOLCHAIN_FILE := ../cmake/toolchainfile-rv1106-glibc.cmake
else
	TOOLCHAIN_FILE := ../cmake/toolchainfile-rv1106.cmake
endif

# Set the install prefix
INSTALL_PREFIX := $(PKG_BIN)

# Set the source directory
SOURCE_DIR := ..

ifeq ($(RK_LVGL_APP_CONFIG),y)
	PKG_TARGET := lvgl_app-build
endif

all: $(PKG_TARGET)
	@echo "build $(PKG_NAME) done"

lvgl_app-build:
	@echo "Creating output directory..."
	@mkdir -p output
	@cd output && cmake -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN_FILE) -DCMAKE_INSTALL_PREFIX="$(CURRENT_DIR)/$(PKG_BIN)" $(SOURCE_DIR)
	@make -C output -j $(nproc)
	@make -C output install
	@mkdir -p $(PKG_BIN)/usr/share && cp -vf $(CURRENT_DIR)/files/* $(PKG_BIN)/usr/share
	@cp -vf $(CURRENT_DIR)/lvgl_app.sh $(PKG_BIN)/usr/bin
	@cp -vf $(CURRENT_DIR)/network.sh $(PKG_BIN)/usr/bin
	$(call MAROC_COPY_PKG_TO_APP_OUTPUT, $(RK_APP_OUTPUT), $(PKG_BIN))

install:
	@make -C output install

clean:
	@rm -rf output
	@rm -rf $(CURRENT_DIR)/$(PKG_BIN)


