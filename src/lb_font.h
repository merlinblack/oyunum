#ifndef LB_FONT_H
#define LB_FONT_H

#include <allegro5/allegro_font.h>
#include "LuaBinding.h"

struct FontBinding : public PODBinding<FontBinding,ALLEGRO_FONT*>
{
    static constexpr const char* class_name = "AllegroFont";

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
            { nullptr, nullptr, nullptr }
        };
        return properties;
    }

    static int create( lua_State* L )
    {
        return luaL_error( L, "Not implemented yet" );
    }

};

#endif //LB_FONT_H
