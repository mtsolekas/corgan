CC = gcc

CFLAGS = -std=gnu11 -rdynamic -Wall -Wextra -Wpedantic -pedantic

CPPFLAGS = -I. `pkg-config --cflags gtk+-3.0`

LDFLAGS = -L.
LDLIBS = `pkg-config --libs gtk+-3.0`

DBGFLAGS = -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer
DBGFLAGS += -Og -ggdb3

OPTFLAGS = -O3 -march=native

DBG ?= 1
ifeq ($(DBG), 0)
CFLAGS += $(OPTFLAGS)
else
CFLAGS += $(DBGFLAGS)
endif

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

BIN_DIR = $(HOME)/.local/bin
DATA_DIR = $(HOME)/.local/share/corgan

.PHONY: all clean install uninstall local_install local_uninstall

all: corgan

corgan: $(OBJECTS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -o corgan $(OBJECTS)

clean:
	-rm corgan
	-rm *.[ao]

install: local_install

uninstall: local_uninstall

local_install:
	mkdir -p $(BIN_DIR)
	mkdir -p $(DATA_DIR)
	cp corgan $(BIN_DIR)/corgan
	cp corgan.glade $(DATA_DIR)/corgan.glade

local_uninstall:
	-rm $(BIN_DIR)/corgan
	-rm -r $(DATA_DIR)
