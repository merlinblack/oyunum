// Binding boiler plate code for copying.
#ifndef LB_MYCLASS_H
#define LB_MYCLASS_H

#include "LuaBinding.h"

using namespace ManualBind;

struct MyClassBinding : public Binding<MyClassBinding,MyClass>
{
    static constexpr const char* class_name = "MyClass";

    static luaL_Reg* members()
    {
        static luaL_Reg members[] = {
            { "__upcast", upcast },
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
        MyClassPtr b = std::make_shared<MyClass>();

        push( L, b );

        return 1;
    }

    static int upcast( lua_State* L )
    {
        MyClassPtr b = fromStack( L, 1 );

        BasePtr r = std::dynamic_pointer_cast<Base>( b );

        BaseBinding::push( L, r );

        return 1;
    }

};

#endif //LB_MYCLASS_H
