ifeq ($(SYSDRV_PARAM), )
    SYSDRV_PARAM:=../../../Makefile.param
    include $(SYSDRV_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)
PKG_BIN := out

all:
	@mkdir -p $(CURRENT_DIR)/$(PKG_BIN)/etc/init.d
	@cp -fv $(CURRENT_DIR)/S51net4g $(CURRENT_DIR)/$(PKG_BIN)/etc/init.d/S51net4g
	@cp -fv $(CURRENT_DIR)/S22spaker $(CURRENT_DIR)/$(PKG_BIN)/etc/init.d/S22spaker
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(SYSDRV_DIR_OUT_ROOTFS), $(PKG_BIN))

clean: distclean

distclean:
	-rm -rf $(PKG_BIN)
