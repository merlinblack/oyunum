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
            { "r", get_rgba, set_rgba },
            { "g", get_rgba, set_rgba },
            { "b", get_rgba, set_rgba },
            { "a", get_rgba, set_rgba },
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

    static const char* prop_keys[];

    static int get_rgba( lua_State* L )
    {
        unsigned char r, g, b, a;

        ALLEGRO_COLOR& c = fromStack( L, 1 );

        al_unmap_rgba( c, &r, &g, &b, &a );

        int which = luaL_checkoption( L, 2, NULL, ColorBinding::prop_keys );

        switch( which )
        {
            case 0:
                lua_pushinteger( L, r );
                break;

            case 1:
                lua_pushinteger( L, g );
                break;

            case 2:
                lua_pushinteger( L, b );
                break;

            case 3:
                lua_pushinteger( L, a );
                break;

        }

        return 1;
    }

    static int set_rgba( lua_State* L )
    {
        unsigned char r, g, b, a;

        ALLEGRO_COLOR& c = fromStack( L, 1 );

        int which = luaL_checkoption( L, 2, NULL, ColorBinding::prop_keys );

        al_unmap_rgba( c, &r, &g, &b, &a );

        switch( which )
        {
            case 0:
                r = luaL_checkinteger( L, 3 );
                break;

            case 1:
                g = luaL_checkinteger( L, 3 );
                break;

            case 2:
                b = luaL_checkinteger( L, 3 );
                break;

            case 3:
                a = luaL_checkinteger( L, 3 );
                break;
        }

        c = al_map_rgba( r, g, b, a );

        return 0;
    }

};

#endif //LB_COLOR_H
