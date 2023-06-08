SRCDIR = src
HEADDIR = include
LIBDIR = ./src

CFLAGS = -W -Wall -Wextra -Wvla -fsanitize=address $(shell sdl2-config --cflags) -lSDL2 -g -lm

LDFLAGS = -export-dynamic $(shell sdl2-config --libs)
DEPENDENCIES = $(SRCDIR)/vect2.c $(SRCDIR)/point.c $(SRCDIR)/spring.c

build:
	gcc $(SRCDIR)/engine.c -o ./bin/square $(DEPENDENCIES) $(CFLAGS) 

run:
	./bin/square 4 4

clean:
	rm ./bin/square

all:
	make build
	make run 