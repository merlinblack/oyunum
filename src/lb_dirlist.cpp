#include "lb_dirlist.h"
#include <allegro5/allegro.h>
#include "lua.hpp"

int dirlist( lua_State* L )
{
    const char* name = luaL_checkstring( L, 1 );

    lua_newtable( L );

    ALLEGRO_FS_ENTRY* dir = al_create_fs_entry( name );

    if( al_open_directory( dir ) )
    {
        ALLEGRO_FS_ENTRY* entry;
        while( (entry = al_read_directory( dir )) )
        {
            lua_newtable( L );
            lua_pushliteral( L, "name" );
            lua_pushstring( L, al_get_fs_entry_name( entry ) );
            lua_settable( L, -3 );

            int mode = al_get_fs_entry_mode(entry);
            lua_pushliteral( L, "dir" );
            lua_pushboolean( L, mode & ALLEGRO_FILEMODE_ISDIR );
            lua_settable( L, -3 );

            off_t size = al_get_fs_entry_size(entry);
            lua_pushliteral( L, "size" );
            lua_pushnumber( L, size );
            lua_settable( L, -3 );

            luaL_ref( L, -2 );
        }
    }

    al_destroy_fs_entry(dir);

    return 1;
}

void dirlist_register( lua_State* L )
{
    lua_register( L, "dirlist", dirlist );
}
