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

.PHONY: all clean

all: corgan

corgan: $(OBJECTS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -o corgan $(OBJECTS)

clean:
	-rm *.[ao]
