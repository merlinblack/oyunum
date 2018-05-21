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
    cout << "Usage: " << argv[0] << "[-r code] file.lua [file.lua ...]" << endl;
    return EXIT_SUCCESS;
}

void initialiseAllegro()
{
    al_init();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
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

    lua_register( L, "print", print );

    register_all_classes( L );

    luaL_dostring( L, "package.path = './scripts/?.lua;' .. package.path" );
    luaL_dostring( L, "package.cpath = './scripts/?.so;' .. package.path" );

    return L;
}

// Run the chunk at the top of the stack.
// If it yields - just call again.
int runcode( lua_State* L )
{
    int ret;
    while( (ret = lua_resume( L, nullptr, 0 ) ) == LUA_YIELD )
    {
        lua_settop( L, 0 );
    }
    return ret;
}

int main( int argc, char* argv[] )
{
    cout << "Koş ay betiği" << endl;
    cout << "Version: " << GIT_REPO_VERSION_STR << endl;
    
    if( argc < 2 )
        return usage( argv );

    initialiseAllegro();
    lua_State* L = initialiseLua();

    int begin = 1;

    if( std::string(argv[1]) == "-r" )
    {
        begin = 3;
        cout << "Executing code from command line." << std::endl;
        if( luaL_loadstring( L, argv[2] ) != LUA_OK )
        {
            std::cerr << lua_tostring( L, -1 ) << std::endl;

            lua_close( L );
            return EXIT_FAILURE;
        }
        if( runcode( L ) != LUA_OK )
        {
            std::cerr << lua_tostring( L, -1 ) << std::endl;

            lua_close( L );
            return EXIT_FAILURE;
        }
    }

    for( int i = begin; i < argc; i++ )
    {
        cout << "Executing Lua file: " << argv[i] << std::endl;
        if( luaL_loadfile( L, argv[i] ) != LUA_OK )
        {
            std::cerr << lua_tostring( L, -1 ) << std::endl;

            lua_close( L );
            return EXIT_FAILURE;
        }
        if( runcode( L ) != LUA_OK )
        {
            std::cerr << lua_tostring( L, -1 ) << std::endl;

            lua_close( L );
            return EXIT_FAILURE;
        }
    }

    cout << "Lua Mem " << lua_gc( L, LUA_GCCOUNT, 0 ) << "kb" << endl;

    lua_close( L );
    return EXIT_SUCCESS;
}
