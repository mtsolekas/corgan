CC = gcc

LIBS = -I. `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`

WARNINGS = -Wall -Wextra -Wpedantic -pedantic

CFLAGS = -std=gnu11 -rdynamic $(WARNINGS) $(LIBS)

OPTFLAGS = -O3 -march=native

SANITIZE = -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer
DBGFLAGS = -Og -ggdb3 $(SANITIZE)

DBG ?= 1
ifeq ($(DBG), 0)
CFLAGS += $(OPTFLAGS)
else
CFLAGS += $(DBGFLAGS)
endif

C_SOURCES = $(wildcard corgan*.c)
C_OBJECTS = $(C_SOURCES:.c=.o)

.PHONY: all clean

all: corgan

corgan: $(C_OBJECTS)
	$(CC) $(CFLAGS) -o corgan $(C_OBJECTS)

clean:
	-rm *.[ao]
