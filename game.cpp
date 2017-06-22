#include <iostream>
#include "game.h"

int main( int argc, char** argv )
{
    Game game;

    if( !game.boot() ) {
        std::cerr << "Could not init display." << std::endl;
        return 1;
    }

    game.setScene();

    game.run();
}
