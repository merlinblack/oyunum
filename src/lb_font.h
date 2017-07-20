#ifndef LB_FONT_H
#define LB_FONT_H

#include <allegro5/allegro_font.h>
#include "LuaBinding.h"

using namespace ManualBind;

struct FontBinding : public PODBinding<FontBinding,ALLEGRO_FONT*>
{
    static constexpr const char* class_name = "AllegroFont";

    static luaL_Reg* members()
    {
        static luaL_Reg members[] = {
            { "__gc", cleanup },
            { nullptr, nullptr }
        };
        return members;
    }

    static bind_properties* properties()
    {
        static bind_properties properties[] = {
            { nullptr, nullptr, nullptr }
        };
        return properties;
    }

    static int create( lua_State* L )
    {
        const char* filename = luaL_checkstring( L, 1 );
        int size = luaL_checknumber( L, 2 );

        ALLEGRO_FONT* font = al_load_font( filename, size, 0 );

        if( font ) {
            push( L, font );
        }
        else
        {
            lua_pushnil( L );
        }

        return 1;
    }

    static int cleanup( lua_State* L )
    {
        ALLEGRO_FONT* font = fromStack( L, 1 );

        al_destroy_font( font );
    }

};

#endif //LB_FONT_H
