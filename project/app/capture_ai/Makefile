# Makefile for capture_ai application
ifeq ($(APP_PARAM), )
	APP_PARAM:=../Makefile.param
	include $(APP_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)

PKG_INSTALL_TO_ROOTFS ?= YES
PKG_NAME := capture_ai
PKG_BIN ?= out
PKG_BUILD ?= build

# Set the path to the CMake toolchain file
TOOLCHAIN_FILE := cmake/toolchainfile-rv1106.cmake

ifeq ($(RK_CAPTURE_AI_CONFIG),y)
	PKG_TARGET := capture_ai-build
endif

all: $(PKG_TARGET)
	@echo "build $(PKG_NAME) done"

capture_ai-build:
	@echo "Creating output directory..."
	@mkdir -p $(PKG_BUILD)
	@cd $(PKG_BUILD) && cmake -DCMAKE_TOOLCHAIN_FILE=$(CURRENT_DIR)/$(TOOLCHAIN_FILE) \
		-DCMAKE_INSTALL_PREFIX="$(CURRENT_DIR)/$(PKG_BIN)" \
		$(CURRENT_DIR)
	@make -C $(PKG_BUILD) -j $$(nproc)
	@mkdir -p $(CURRENT_DIR)/$(PKG_BIN)/usr/bin
	@cp -vf $(PKG_BUILD)/capture_ai $(CURRENT_DIR)/$(PKG_BIN)/usr/bin/
	@cp -vf $(CURRENT_DIR)/RkLunch.sh $(PKG_BIN)/usr/bin
	@cp -vf $(CURRENT_DIR)/RkLunch-stop.sh $(PKG_BIN)/usr/bin
	@mkdir -p $(CURRENT_DIR)/$(PKG_BIN)/share/
	@cp -vf $(CURRENT_DIR)/model/yolov5n-face-rv1106.rknn $(CURRENT_DIR)/$(PKG_BIN)/share/
	$(call MAROC_COPY_PKG_TO_APP_OUTPUT, $(RK_APP_OUTPUT), $(PKG_BIN))

install:
	@make -C $(PKG_BUILD) install

clean:
	@rm -rf $(PKG_BUILD)
	@rm -rf $(CURRENT_DIR)/$(PKG_BIN)