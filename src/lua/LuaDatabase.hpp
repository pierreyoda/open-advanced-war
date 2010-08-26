#ifndef LUADATABASE_HPP
#define LUADATABASE_HPP

/** \file LuaDatabase.hpp
* \brief Binds database to Lua.
*/

extern "C"
{
    #include <lua.h>
    #include <lualib.h>
}
#include <luabind/luabind.hpp>
#include "../db/Database.hpp"

#ifdef DB_EXPORTER
    #define db_member def_readwrite // Full access
#else
    #define db_member def_readonly // Read-only access
#endif /* DB_EXPORTER */

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
        , class_<Database>("Database")
            .def("getModuleName", &Database::getModuleName)
            .def("addTile", &Database::addTile)
            .def("findTile", (Tile*(Database::*)(const std::string&))
                &Database::findTile)
        // TranslationProvider
        , class_<TranslationProvider>("TranslationProvider")
            .def("tr", (std::string(TranslationProvider::*)(const std::string&))
                &TranslationProvider::tr)
            .def("tr", (std::string(TranslationProvider::*)(const std::string&,
                    const bool&))&TranslationProvider::tr)
            .def("selectLang", &TranslationProvider::selectLang)
            .def("translateItem", &TranslationProvider::translateItem)
        // Frame
        , class_<Frame>("Frame")
            .def(constructor<const unsigned int&, const unsigned int&,
                const float&>())
            .def(constructor<const unsigned int&, const unsigned int&,
                const unsigned int&, const unsigned int&, const float&>())
            .db_member("x", &Frame::x)
            .db_member("y", &Frame::y)
            .db_member("w", &Frame::w)
            .db_member("h", &Frame::h)
            .db_member("duration", &Frame::duration)
        // Animation
        , class_<Animation, bases<DatabaseItem> >("Anim")
            .def(constructor<const std::string&, const std::string&>())
            .def("addFrame", (Animation&(Animation::*)(const Frame&))
                &Animation::addFrame)
            .def("addFrame", (Animation&(Animation::*)(const unsigned int &,
                const unsigned int&))&Animation::addFrame)
            .def("addFrame", (Animation&(Animation::*)(const unsigned int &,
                const unsigned int&, const float&))&Animation::addFrame)
            .def("setImage", &Animation::setImage)
            .def("clear", &Animation::clear)
            .def("image", &Animation::image)
        // XSpriteItem
        , class_<XSpriteItem, bases<DatabaseItem> >("XSprite")
            .def("addAnim", &XSpriteItem::addAnim)
        // Tile
        , class_<Tile, bases<XSpriteItem> >("Tile")
            .def(constructor<const std::string&>())
            .def(constructor<const std::string&, const bool&>())
            .def("setProtection", &Tile::setProtection)
            .def("isOrientable", &Tile::isOrientable)
            .def("protection", &Tile::protection)
    ];
}

#endif /* LUADATABASE_HPP */
