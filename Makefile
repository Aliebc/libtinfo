# LIBTINFO MAKEFILE
# For Darwin Universal (x86_64 and arm64)
# Copyright (C) 2022 LIBTINFO
# By Aliebc

CC=clang
LIPO=lipo
INSTALL_PREFIX=/usr/local/lib
CURL_PATH=/usr
CURL_LIBRARY_PATH=$(CURL_PATH)/lib
CURL_INCLUDE_PATH=$(CURL_PATH)/include
CFLAGS= -std=c99 -I.  -I$(CURL_INCLUDE_PATH) -L$(CURL_LIBRARY_PATH) -lcurl -shared -fPIC -O3
TARGET_NAME=libtinfo
TARGET_SUFFIX=dylib
X64_TARGET=TMP_X64/$(TARGET_NAME).$(TARGET_SUFFIX)
ARM64_TARGET=TMP_ARM64/$(TARGET_NAME).$(TARGET_SUFFIX)
TARGET=$(TARGET_NAME).$(TARGET_SUFFIX)
CFILE=tinfo.c cJSON.c
EXPFILE=example.c
EXP_CFLAGS= -I. -L. -L$(CURL_LIBRARY_PATH) -lcurl -ltinfo 

$(TARGET):$(CFILE)
	mkdir -p TMP_X64
	mkdir -p TMP_ARM64
	$(CC) -o $(X64_TARGET) $(CFILE) $(CFLAGS) --target=x86_64-apple-darwin
	$(CC) -o $(ARM64_TARGET) $(CFILE) $(CFLAGS) --target=arm64-apple-darwin
	$(LIPO) -create -output $(TARGET) $(X64_TARGET) $(ARM64_TARGET)
	-$(RM) $(X64_TARGET)
	-$(RM) $(ARM64_TARGET)
	$(CC) -o example $(EXPFILE) $(EXP_CFLAGS)

clean:
	-$(RM) $(TARGET)
	-$(RM) example

install:
	cp $(TARGET) $(INSTALL_PREFIX)