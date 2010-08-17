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
        // DatabaseItem
        class_<DatabaseItem>("DatabaseItem")
            .def("name", &DatabaseItem::name)
        // Database
        //class_<Database>("Database")
        // Frame
        , class_<Frame>("Frame")
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
        , class_<Animation, bases<DatabaseItem> >("Anim")
            .def(constructor<const std::string&, const std::string&>())
            .def("addFrame", &Animation::addFrame)
            .def("image", &Animation::image)
        // XSpriteItem
        , class_<XSpriteItem, bases<DatabaseItem> >("XSprite")
            .def("addAnim", &XSpriteItem::addAnim)
        // Tile
        , class_<Tile, bases<XSpriteItem> >("Tile")
            .def(constructor<const std::string&>())
            .def(constructor<const std::string&, const bool&>())
            .def("setProtection", &Tile::setProtection)
    ];
}

#endif /* LUADATABASE_HPP */
