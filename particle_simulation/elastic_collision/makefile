SRCDIR = src
HEADDIR = include
LIBDIR = ./src

CFLAGS = -W -Wall -Wextra -Wvla -fsanitize=address $(shell sdl2-config --cflags) -lSDL2 -lm

LDFLAGS = -export-dynamic $(shell sdl2-config --libs)
DEPENDENCIES = $(SRCDIR)/engine.c $(SRCDIR)/geometry.c $(SRCDIR)/vect2.c $(SRCDIR)/ball.c

build:
	gcc $(SRCDIR)/elastic_collision.c -o ./elastic_collision $(DEPENDENCIES) $(CFLAGS) 

run:
	./elastic_collision 6

clean:
	rm elastic_collision

all:
	make build
	make run
	clear