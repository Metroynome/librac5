HOST_CC ?= cc
CFLAGS = -std=c99 -O2 -Wall -Wextra -Werror
.PHONY: all test
all: bin/symbols_test
bin/symbols_test: symbols_test.c $(wildcard ../src/*.c) $(wildcard ../include/*.h) ../src/region_profiles.inc
	mkdir -p bin
	$(HOST_CC) $(CFLAGS) -I../include symbols_test.c ../src/*.c -o $@
test: all
	./bin/symbols_test $(DUMP)

PYTHON ?= python3
.PHONY: regions
regions:
	$(PYTHON) regions_test.py --cc "$(HOST_CC)"
