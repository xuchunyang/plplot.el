# -*- mode: makefile-gmake; -*-

EMACS := emacs

ifeq ($(OS),Windows_NT)
	EXT = dll
else
	EXT = so		# both .so and .dylib work on macOS
endif

override CFLAGS += -Wall -shared -fpic -lplplot

plplot-module.$(EXT): plplot-module.c
	$(CC) $(CFLAGS) $< -o $@

%.elc: %.el
	$(EMACS) -Q --batch -L . --eval "(setq byte-compile-error-on-warn t)" -f batch-byte-compile $<

check: plplot-module.$(EXT) plplot.elc
	$(EMACS) -Q --batch -L . -l *-tests.el -f ert-run-tests-batch-and-exit
