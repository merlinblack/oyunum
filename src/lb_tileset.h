
#ifndef LB_TILESET_H
#define LB_TILESET_H

#include "tileset.h"
#include "lb_bitmap.h"
#include "LuaBinding.h"

using namespace ManualBind;

struct TileSetBinding : public Binding<TileSetBinding,TileSet>
{
    static constexpr const char* class_name = "TileSet";

    static luaL_Reg* members()
    {
        static luaL_Reg members[] = {
            { "loadSourceBitmap", loadSourceBitmap },
            { "getTile", getTile },
            { nullptr, nullptr }
        };
        return members;
    }

    static bind_properties* properties()
    {
        return nullptr;
    }

    static int create( lua_State* L )
    {
        int first = luaL_optinteger( L, 1, 1 );
        int width = luaL_optinteger( L, 1, 32 );
        int height = luaL_optinteger( L, 1, 32 );

        TileSetPtr ts = std::make_shared<TileSet>( first, width, height );

        push( L, ts );

        return 1;
    }

    static int loadSourceBitmap( lua_State* L )
    {
        TileSetPtr ts = fromStack( L, 1 );

        std::string filename( lua_tostring( L, 2 ) );

        lua_pushboolean( L, ts->loadSourceBitmap( filename ) );

        return 1;
    }

    static int getTile( lua_State* L )
    {
        TileSetPtr ts = fromStack( L, 1 );
        int tileId = luaL_checkinteger( L , 2 );

        ALLEGRO_BITMAP* ab = ts->getTile( tileId );

        if( ab == nullptr ) {
            lua_pushnil( L );
            return 1;
        }

        BitmapPtr b = std::make_shared<Bitmap>( ab );

        BitmapBinding::push( L, b );

        return 1;
    }

};

#endif //LB_TILESET_H
