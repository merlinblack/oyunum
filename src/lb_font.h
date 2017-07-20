#ifndef LB_FONT_H
#define LB_FONT_H

#include "font.h"
#include "lb_font.h"
#include "LuaBinding.h"

using namespace ManualBind;

struct FontBinding : public Binding<FontBinding,Font>
{
    static constexpr const char* class_name = "Font";

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
        const char* filename = luaL_checkstring( L, 1 );
        int size = luaL_checknumber( L, 2 );

        FontPtr font = std::make_shared<Font>( filename, size, 0 );

        push( L, font );

        return 1;
    }

};

#endif //LB_FONT_H
