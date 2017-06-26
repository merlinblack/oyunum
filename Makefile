ifeq ($(shell uname -s),Darwin)
	LUA=lua5.3
else
	LUA=lua
endif
ALLEGRO=allegro-5 allegro_main-5 allegro_font-5 allegro_image-5 allegro_primitives-5
CCFLAGS+=$(shell pkg-config --cflags $(LUA) $(ALLEGRO) )
LDFLAGS+=$(shell pkg-config   --libs $(LUA) $(ALLEGRO) )

game:	game.cpp *.h Makefile debug.o
	c++ $(CCFLAGS) -std=c++14 -g -O0 game.cpp debug.o $(LDFLAGS) -o game

debug.o:	debug.cpp
	c++ $(CCFLAGS) -std=c++14 -O3 -c debug.cpp -o debug.o

clean:
	rm -f game
	rm -rf game.dSYM
	rm -f debug.o
