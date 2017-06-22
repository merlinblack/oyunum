ifeq ($(shell uname -s),Darwin)
	LUA=lua5.3
else
	LUA=lua
endif
ALLEGRO=allegro-5 allegro_main-5 allegro_font-5 allegro_image-5 allegro_primitives-5
CCFLAGS+=$(shell pkg-config --cflags $(LUA) $(ALLEGRO) )
LDFLAGS+=$(shell pkg-config   --libs $(LUA) $(ALLEGRO) )

game:	game.cpp *.h Makefile
	c++ $(CCFLAGS) -std=c++14 -g -O0 game.cpp $(LDFLAGS) -o game

clean:
	rm -f game
	rm -rf game.dSYM
