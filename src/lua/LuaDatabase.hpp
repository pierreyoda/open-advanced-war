#ifndef LUADATABASE_HPP
#define LUADATABASE_HPP

extern "C"
{
    #include <lua.h>
    #include <lualib.h>
}
#include <luabind/luabind.hpp>
#include "../db/Database.hpp"

void exportDatabase(lua_State *lua)
{
    using namespace db;
    using namespace luabind;
    module(lua, "db")
    [
        // Frame
        class_<Frame>("Frame")
            .def(constructor<const unsigned int&, const unsigned int&,
                const float&>())
            .def(constructor<const unsigned int&, const unsigned int&,
                const unsigned int&, const unsigned int&, const float&>())
            .def_readwrite("x", &Frame::x)
            .def_readwrite("y", &Frame::y)
            .def_readwrite("w", &Frame::w)
            .def_readwrite("h", &Frame::h)
            .def_readwrite("duration", &Frame::duration)
        // Animation
        , class_<Animation>("Anim")
            .def(constructor<const std::string&, const std::string&>())
            .def("addFrame", &Animation::addFrame)
        // XSprite
        , class_<XSpriteItem>("XSprite")
            .def(constructor<const std::string &>())
            .def("addAnim", &XSpriteItem::addAnim)
    ];
}

#endif /* LUADATABASE_HPP */
