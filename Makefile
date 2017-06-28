ifeq ($(shell uname -s),Darwin)
	LUA=lua5.3
else
	LUA=lua
endif
ALLEGRO_LIBS = allegro-5 allegro_main-5 allegro_font-5 allegro_image-5 allegro_primitives-5
CCFLAGS += -std=c++14
CCFLAGS += -Isrc
CCFLAGS += -Imanualbind/src
CCFLAGS += $(shell pkg-config --cflags $(LUA) $(ALLEGRO_LIBS) )
LDFLAGS += $(shell pkg-config   --libs $(LUA) $(ALLEGRO_LIBS) )
VPATH = src:manualbind/src

game:	game.cpp src/debug.o src/*.h Makefile
	c++ $(CCFLAGS) -g -O0 $< src/debug.o $(LDFLAGS) -o $@

src/debug.o:	debug.cpp
	c++ $(CCFLAGS) -O3 -c $< -o $@

clean:
	rm -f game
	rm -rf game.dSYM
	rm -f src/debug.o
