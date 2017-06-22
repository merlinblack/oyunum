ifeq ($(shell uname -s),Darwin)
	CCFLAGS+=-I/usr/local/include
	LDFLAGS+=-L/usr/local/lib
endif

game:	game.cpp *.h Makefile
	c++ $(CCFLAGS) -std=c++14 -g -O0 game.cpp $(LDFLAGS) -llua -lallegro -lallegro_main -lallegro_font -lallegro_image -lallegro_primitives -o game

clean:
	rm -f game
