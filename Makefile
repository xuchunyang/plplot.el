# -*- mode: makefile-gmake; -*-

ifeq ($(OS),Windows_NT)
	EXT = dll
else
	EXT = so		# both .so and .dylib work on macOS
endif

override CFLAGS += -Wall -shared -fpic -lplplot

plplot-module.$(EXT): plplot-module.c
	$(CC) $(CFLAGS) $< -o $@
