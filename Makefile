ifndef DEBUG
DEBUG = 1
endif

CC = gcc

LIBS = -I. `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`
CFLAGS = -std=gnu11 -Wall -Wpedantic -rdynamic

ifeq ($(DEBUG), 1)
CFLAGS += -g3
else
CFLAGS += -O3 -march=native
endif

CFLAGS += $(LIBS)

.PHONY: all clean

all: corgan

corgan: corgan.o corgan_data.o
	$(CC) $(CFLAGS) -o corgan corgan.o corgan_data.o

corgan.o: corgan.c corgan.h
	$(CC) $(CFLAGS) -c corgan.c

corgan_data.o: corgan_data.c corgan_data.h
	$(CC) $(CFLAGS) -c corgan_data.c

clean:
	-rm corgan
	-rm *.[ao]
