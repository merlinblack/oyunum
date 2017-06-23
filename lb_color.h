#ifndef LB_COLOR_H
#define LB_COLOR_H

#include <allegro5/allegro.h>
#include "LuaBinding.h"

struct ColorBinding : public PODBinding<ColorBinding,ALLEGRO_COLOR>
{
    static constexpr const char* class_name = "AllegroColor";

    static luaL_Reg* members()
    {
        static luaL_Reg members[] = {
            { nullptr, nullptr }
        };
        return members;
    }

    static bind_properties* properties()
    {
        static bind_properties properties[] = {
            { "rgba", get_rgba, set_rgba },
            { nullptr, nullptr, nullptr }
        };
        return properties;
    }

    static int create( lua_State* L )
    {
        unsigned char r, g, b, a;

        r = luaL_checkinteger( L, 1 );
        g = luaL_checkinteger( L, 2 );
        b = luaL_checkinteger( L, 3 );
        a = luaL_checkinteger( L, 4 );

        ALLEGRO_COLOR c = al_map_rgba( r, g, b, a );

        push( L, c );

        return 1;
    }

    static int get_rgba( lua_State* L )
    {
        unsigned char r, g, b, a;

        ALLEGRO_COLOR& c = fromStack( L, 1 );

        al_unmap_rgba( c, &r, &g, &b, &a );

        lua_pushinteger( L, r );
        lua_pushinteger( L, g );
        lua_pushinteger( L, b );
        lua_pushinteger( L, a );

        return 4;
    }

    static int set_rgba( lua_State* L )
    {
        unsigned char r, g, b, a;

        ALLEGRO_COLOR& c = fromStack( L, 1 );

        r = luaL_checkinteger( L, 2 );
        g = luaL_checkinteger( L, 3 );
        b = luaL_checkinteger( L, 4 );
        a = luaL_checkinteger( L, 5 );

        c = al_map_rgba( r, g, b, a );

        return 0;
    }

};

#endif //LB_COLOR_HluaL_error( L, "Not implemented yet" );
