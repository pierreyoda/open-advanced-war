#include "LuaBinds.hpp"
#include "../db/Database.hpp"

#ifdef DB_EXPORTER
    #define db_member def_readwrite // Full access
#else
    #define db_member def_readonly // Read-only access
    #include "../Map.hpp"
    #include "../tools/PausableClock.hpp"
    #include "../tools/FilesPathHandler.hpp"
#endif /* DB_EXPORTER */

using namespace db;
using namespace luabind;

void LuaBinds::exportDatabase(lua_State *lua)
{
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

#ifndef DB_EXPORTER

void LuaBinds::exportTools(lua_State *lua)
{
    module(lua)
    [
        // "Namespace" sf (SFML)
        namespace_("sf")
        [
            // sf::Vector2i (sf::Vector2<int>)
            class_<sf::Vector2i>("Vector2i")
                .def(constructor< >())
                .def(constructor<int, int>())
                .def_readwrite("x", &sf::Vector2i::x)
                .def_readwrite("y", &sf::Vector2i::y)
        ]
        // PausableClock
        , class_<PausableClock>("PausableClock")
            .def(constructor< >())
            .def(constructor<bool>())
            .def("pause", &PausableClock::pause)
            .def("start", &PausableClock::start)
            .def("getElapsedTime", &PausableClock::getElapsedTime)
            .def("reset", (void(PausableClock::*)())&PausableClock::reset)
            .def("reset", (void(PausableClock::*)(const bool&))&PausableClock::reset)
        // FilesPathHandler
        , class_<FilesPathHandler>("FilesPathHandler")
            .def("addFile", (void(FilesPathHandler::*)(const std::string&,
                const std::string&))&FilesPathHandler::addFile)
            .def("addFile", (void(FilesPathHandler::*)(const std::string&,
                const std::string&, const bool&))&FilesPathHandler::addFile)
            .def("getFilepath", &FilesPathHandler::getFilepath)
    ];
}

void LuaBinds::exportGame(lua_State *lua)
{
    using namespace luabind;
    module(lua)
    [
        // Map
        class_<Map>("Map")
            .def("getTile", (std::string(Map::*)(const unsigned int&,
                const unsigned int&)const)&Map::getTile)
            .def("setTile", (void(Map::*)(const unsigned int&,
                const unsigned int&, const std::string &))&Map::setTile)
    ];
}

#endif /* DB_EXPORTER */
