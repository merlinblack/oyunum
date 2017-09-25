#include <iostream>
#include <cstdlib>
#include "git_versioning.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include <lua.hpp>
#include "LuaRef.h"

#include "text.h"
#include "font.h"
#include "rectangle.h"
#include "bitmap.h"
#include "tileset.h"
#include "tilegrid.h"

#include "lb_text.h"
#include "lb_font.h"
#include "lb_color.h"

#include "colortext.h"

// in binding.cpp
void register_all_classes( lua_State* L );

using std::cout;
using std::endl;

int usage( char* argv[] )
{
    cout << "Usage: " << argv[0] << " file.lua" << endl;
    return EXIT_SUCCESS;
}

void initialiseAllegro()
{
    al_init();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
}

int fake_yield( lua_State* L )
{
    return 0;
}

int print( lua_State* L )
{
    std::string output;
    int nArgs = lua_gettop( L );

    lua_getglobal( L, "tostring" );

    for( int index = 1; index <= nArgs; index++ )
    {
        const char *s;

        lua_pushvalue( L, -1 ); // tostring func
        lua_pushvalue( L, index );
        lua_pcall( L, 1, 1, 0 );
        s = lua_tostring( L, -1 );

        if( !s ) {
            return luaL_error( L, "'tostring' must return a string to 'print'" );
        }

        output += s;

        if( index < nArgs )
            output += "\t";

        lua_pop( L, 1 );
    }

    cout << strip_color_text(output) << endl;

    return 0;
}

lua_State* initialiseLua()
{
    lua_State* L = luaL_newstate();
    luaL_openlibs( L );

    lua_getglobal( L, "coroutine" );
    lua_pushcfunction( L, fake_yield );
    lua_setfield( L, -2, "yield" );

    lua_register( L, "print", print );

    register_all_classes( L );

    luaL_dostring( L, "package.path = './scripts/?.lua;' .. package.path" );

    return L;
}

int main( int argc, char* argv[] )
{
    cout << "Koş ay betiği" << endl;
    cout << "Version: " << GIT_REPO_VERSION_STR << endl;
    
    if( argc != 2 )
        return usage( argv );

    initialiseAllegro();
    lua_State* L = initialiseLua();

    if( luaL_dofile( L, argv[1] ) )
    {
        std::cerr << lua_tostring( L, -1 ) << std::endl;

        lua_close( L );
        return EXIT_FAILURE;
    }

    cout << "Lua Mem " << lua_gc( L, LUA_GCCOUNT, 0 ) << "kb" << endl;

    lua_close( L );
    return EXIT_SUCCESS;
}
