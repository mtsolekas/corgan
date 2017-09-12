CC = gcc

SANITIZE = -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer
WARNINGS = -Wall -Wextra -Wpedantic -pedantic
DIRS = -I. `pkg-config --cflags gtk+-3.0`

DBGFLAGS = -Og -ggdb3 $(SANITIZE)
OPTFLAGS = -O3 -march=native

CFLAGS = -std=gnu11 -rdynamic $(WARNINGS) $(DIRS)
LDFLAGS = `pkg-config --libs gtk+-3.0`

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
	$(CC) $(CFLAGS) $(LDFLAGS) -o corgan $(C_OBJECTS)

clean:
	-rm *.[ao]
