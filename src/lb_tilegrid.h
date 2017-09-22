#ifndef LB_TILEGRID_H
#define LB_TILEGRID_H

#include "LuaBinding.h"
#include "lb_renderlist.h"
#include "lb_tileset.h"
#include "tilegrid.h"

using namespace ManualBind;

struct TileGridBinding : public Binding<TileGridBinding,TileGrid>
{
    static constexpr const char* class_name = "TileGrid";

    static luaL_Reg* members()
    {
        static luaL_Reg members[] = {
            { "__upcast", upcast },
            { "setXY", setXY },
            { "setData", setData },
            { nullptr, nullptr }
        };
        return members;
    }

    static int create( lua_State* L )
    {
        TileSetPtr ts = TileSetBinding::fromStack( L, 1 );
        int w = luaL_checkinteger( L, 2 );
        int h = luaL_checkinteger( L, 3 );

        TileGridPtr tg = std::make_shared<TileGrid>( ts, w, h );

        push( L, tg );

        return 1;
    }

    int static upcast( lua_State* L )
    {
        TileGridPtr tg = fromStack( L, 1 );

        RenderablePtr r = std::dynamic_pointer_cast<Renderable>( tg );

        RenderableBinding::push( L, r );

        return 1;
    }

    int static setXY( lua_State* L )
    {
        TileGridPtr tg = fromStack( L, 1 );
        int x = luaL_checkinteger( L, 2 );
        int y = luaL_checkinteger( L, 3 );

        tg->setXY( x, y );

        return 0;
    }

    int static setData( lua_State* L )
    {
        TileGridPtr tg = fromStack( L, 1 );
        int tile;
        TileGridData newData;

        if( ! lua_istable( L, 2 ) )
        {
            return luaL_error( L, "You should pass a table of integers to TileGrid::loadData" );
        }

        lua_pushnil( L );

        while( lua_next( L, 2 ) )
        {
            tile = luaL_checkinteger( L, -1 );
            newData.push_back( tile );
            lua_pop( L, 1 );
        }

        tg->setData( std::move(newData) );

        return 0;
    }

};

#endif //LB_TILEGRID_H
