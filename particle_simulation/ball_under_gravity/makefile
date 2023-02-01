SRCDIR = src
HEADDIR = include
LIBDIR = ./src

CFLAGS = -W -Wall -Wextra -Wvla -fsanitize=address $(shell sdl2-config --cflags) -lSDL2 -lm

LDFLAGS = -export-dynamic $(shell sdl2-config --libs)
DEPENDENCIES = $(SRCDIR)/ball.c $(SRCDIR)/engine.c

build:
	gcc $(SRCDIR)/ball_under_gravity.c -o ./ball_under_gravity $(CFLAGS) $(DEPENDENCIES)

run: ./ball_under_gravity
	./ball_under_gravity

clean:
	rm ball_under_gravity

all:
	make build
	make run