ifndef DEBUG
DEBUG = 1
endif

CC = gcc

LIBS = -I. `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`

WARNINGS = -Wall -Wextra -Wpedantic

CFLAGS = -std=gnu11 -rdynamic $(WARNINGS) $(LIBS)

ifeq ($(DEBUG), 1)
SANITIZE = -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer
CFLAGS += -g3 $(SANITIZE)
else
CFLAGS += -O3 -march=native
endif

C_SOURCES = $(wildcard corgan*.c)
C_OBJECTS = $(C_SOURCES:.c=.o)

.PHONY: clean

corgan: $(C_OBJECTS)
	$(CC) $(CFLAGS) -o corgan $(C_OBJECTS)

clean:
	-rm *.[ao]
