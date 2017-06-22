
game:	game.cpp *.h Makefile
	c++ -std=c++14 -g -O0 game.cpp -llua -lallegro -lallegro_main -lallegro_font -lallegro_image -lallegro_primitives -o game

clean:
	rm -f game
