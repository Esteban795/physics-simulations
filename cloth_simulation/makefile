SRCDIR = src
HEADDIR = include
LIBDIR = ./src

CFLAGS = -W -Wall -Wextra -Wvla -fsanitize=address $(shell sdl2-config --cflags) -lSDL2 -lm

LDFLAGS = -export-dynamic $(shell sdl2-config --libs)
DEPENDENCIES = $(SRCDIR)/vect2.c $(SRCDIR)/point.c $(SRCDIR)/stick.c $(SRCDIR)/engine.c $(SRCDIR)/cloth.c $(SRCDIR)/mouse.c

build:
	gcc $(SRCDIR)/cloth_simulation.c -o ./bin/cloth_simulation $(DEPENDENCIES) $(CFLAGS) 

run:
	./bin/cloth_simulation $(ROWS) $(COLUMNS)

clean:
	rm ./bin/cloth_simulation

all:
	make build
	make run ROWS="30" COLUMNS="60"
