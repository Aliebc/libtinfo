# LIBTINFO MAKEFILE
# For MacOS/Windows/Linux
# Copyright (C) 2022 LIBTINFO
# By Aliebc

CC=gcc
PRE_BUILD=pre-build/
RM=/bin/rm
CP=cp
TAR=tar
MKDIR=mkdir
HOST=$(shell $(CC) -dumpmachine)
LIBEXT=
SLIBEXT=
CURL_DIR=
CURL_CFLAGS=-DCURL_STATICLIB $(shell $(CURL_DIR)/bin/curl-config --static-libs)
CURL_INCLUDE=$(shell $(CURL_DIR)/bin/curl-config --prefix)/include
OPENSSL_DIR=
TINFO_CFLAGS='-DTINFO_PLATFORM="$(HOST)"'
CFLAGS=
U_CFLAGS=-fPIC -O2 -Wall
ARCH=$(shell arch)
LDFLAGS=
D_NAME=libtinfo
ifneq (,$(findstring darwin,$(HOST)))
LIBEXT=dylib
SLIBEXT=a
DYLD_RNAME=$(D_NAME)-1_1.$(LIBEXT)
DYLD_NAME=$(PRE_BUILD)lib/$(D_NAME).$(LIBEXT)
endif
ifneq (,$(findstring linux,$(HOST)))
LIBEXT=so
SLIBEXT=a
DYLD_RNAME=$(D_NAME)-1_1.$(LIBEXT)
DYLD_NAME=$(PRE_BUILD)lib/$(D_NAME).$(LIBEXT)
endif
ifneq (,$(findstring mingw32,$(HOST)))
LIBEXT=dll
SLIBEXT=dll.a
DYLD_RNAME=$(D_NAME)-1_1.$(LIBEXT)
DYLD_NAME=$(PRE_BUILD)bin/$(D_NAME)-1_1.$(LIBEXT)
LDFLAGS=-Wl,--out-implib,$(PRE_BUILD)lib/libtinfo.$(SLIBEXT)
endif
INSTALL_DIR=/usr/local/

build:
	$(MKDIR) -p $(PRE_BUILD) $(PRE_BUILD)bin $(PRE_BUILD)lib $(PRE_BUILD)include $(PRE_BUILD)share
	$(CC) -I. -I $(CURL_INCLUDE) tinfo.c cJSON.c -o $(DYLD_NAME) -shared $(U_CFLAGS) $(TINFO_CFLAGS) $(CURL_CFLAGS) $(CFLAGS)\
	$(LDFLAGS)
	cp tinfo.h $(PRE_BUILD)include
	$(CC) example.c -I $(PRE_BUILD)include -L $(PRE_BUILD)lib -ltinfo  -o $(PRE_BUILD)bin/tinfo

install:
	$(CP) $(PRE_BUILD)bin/* $(INSTALL_DIR)bin
	$(CP) $(PRE_BUILD)include/* $(INSTALL_DIR)include
	$(CP) $(PRE_BUILD)lib/* $(INSTALL_DIR)lib
	$(CP) $(PRE_BUILD)share/* $(INSTALL_DIR)share

clean:
	$(RM) -rf $(PRE_BUILD)

remove:
	$(RM) -f $(INSTALL_DIR)bin/tinfo
	$(RM) -f $(INSTALL_DIR)include/tinfo.h
	$(RM) -f $(INSTALL_DIR)lib/$(DYLD_RNAME)

dist:
	mv $(PRE_BUILD) libtinfo-$(HOST)
	$(TAR) -czvf libtinfo-$(HOST).tar.gz libtinfo-$(HOST)/
	mv libtinfo-$(HOST) $(PRE_BUILD)