
ifeq ($(APP_PARAM), )
    APP_PARAM:=../Makefile.param
    include $(APP_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

ifeq ($(RK_APP_ARCH_TYPE),arm)
PKG_USE_THUMB2 ?= YES
endif

CURRENT_DIR := $(shell pwd)

PKG_NAME := gb28181_sip
PKG_BIN ?= out

PKG_MXML := mxml-4.0.3
PKG_LIBOSIP2 := libosip2-4.1.0
PKG_LIBEXOSIP2 := libeXosip2-4.1.0

ifeq ($(RK_GB28181_SIP),y)
PKG_TARGET := libosip2-build libexosip2-build mxml-build sip-build
endif

ifeq ($(PKG_BIN),)
$(error ### $(CURRENT_DIR): PKG_BIN is NULL, Please Check !!!)
endif

all: $(PKG_TARGET)
	@echo "build $(PKG_NAME) done"

sip-build:
	rm -rf $(PKG_BIN);
	mkdir -p $(PKG_BIN);

	make -C $(PKG_NAME)/
	mkdir -p $(PKG_BIN)/bin ;\
	cp $(PKG_NAME)/build/gb28181-sip $(PKG_BIN)/bin/ ;

	pushd $(PKG_NAME)/; \
		rm -rf share ; \
		rm -rf lib/pkgconfig ; \
		rm lib/*.la || true ; \
		rm lib/*.a || true ; \
	popd;
	cp -rv $(PKG_NAME)/lib $(PKG_BIN)/ ;
	mkdir -p $(PKG_BIN)/share
	cp -rv $(PKG_NAME)/samples/0.h264 $(PKG_BIN)/share ;

	$(call MAROC_COPY_PKG_TO_APP_OUTPUT, $(RK_APP_OUTPUT), $(PKG_BIN))

libosip2-build:
	@test -d $(CURRENT_DIR)/$(PKG_LIBOSIP2) || (\
		rm -rf $(PKG_LIBOSIP2); \
		echo "Uncompress libosip2"; \
		mkdir -p $(PKG_LIBOSIP2); \
		tar xzvf $(CURRENT_DIR)/$(PKG_NAME)/3rd_party/$(PKG_LIBOSIP2).tar.gz -C $(CURRENT_DIR); \
		exit 0; )

	@test -f $(PKG_NAME)/lib/libosip2.so || (\
		pushd $(PKG_LIBOSIP2); \
			./configure --host=$(RK_APP_CROSS) \
						--prefix=$(CURRENT_DIR)/$(PKG_NAME)/; \
			make -j$(RK_APP_JOBS) || exit -1; \
			make install; \
		popd; )

libexosip2-build:
	@test -d $(CURRENT_DIR)/$(PKG_LIBEXOSIP2) || (\
		rm -rf $(PKG_LIBEXOSIP2); \
		echo "Uncompress libexosip2"; \
		mkdir -p $(PKG_LIBEXOSIP2); \
		tar xzvf $(CURRENT_DIR)/$(PKG_NAME)/3rd_party/$(PKG_LIBEXOSIP2).tar.gz -C $(CURRENT_DIR); \
		exit 0; )

	@test -f $(PKG_NAME)/lib/libeXosip2.so || (\
		pushd $(PKG_LIBEXOSIP2); \
			./configure --host=$(RK_APP_CROSS) \
						--disable-openssl \
						--prefix=$(CURRENT_DIR)/$(PKG_NAME)/ \
						LDFLAGS="-L$(CURRENT_DIR)/$(PKG_NAME)/lib" \
						CFLAGS="-L$(CURRENT_DIR)/$(PKG_NAME)/include"; \
			make -j$(RK_APP_JOBS) || exit -1; \
			make install; \
		popd; )

mxml-build:
	@test -d $(CURRENT_DIR)/$(PKG_MXML) || (\
		rm -rf $(PKG_MXML); \
		echo "Uncompress libmxml4"; \
		mkdir -p $(PKG_MXML); \
		tar xzvf $(CURRENT_DIR)/$(PKG_NAME)/3rd_party/$(PKG_MXML).tar.gz -C $(CURRENT_DIR); \
		exit 0; )

	@test -f $(PKG_NAME)/lib/libmxml4.so || (\
		pushd $(PKG_MXML); \
			./configure --host=$(RK_APP_CROSS) \
						--prefix=$(CURRENT_DIR)/$(PKG_NAME)/; \
			make -j$(RK_APP_JOBS) || exit -1; \
			make install; \
		popd; )

clean:
	@rm -rf $(PKG_BIN) $(PKG_LIBOSIP2) $(PKG_LIBEXOSIP2) $(PKG_MXML)
	@make clean -C $(PKG_NAME)/
	@rm -rf $(PKG_NAME)/lib

distclean: clean

info:
ifeq ($(RK_GB28181_SIP),y)
	@echo -e "$(C_YELLOW)-------------------------------------------------------------------------$(C_NORMAL)"
	@echo -e "	RK_GB28181_SIP=y"
	@echo -e "$(C_YELLOW)-------------------------------------------------------------------------$(C_NORMAL)"
endif
