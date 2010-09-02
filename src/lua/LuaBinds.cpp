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
            .def("addUnit", &Database::addUnit)
            .def("addBuilding", &Database::addBuilding)
            .def("addCategory", &Database::addCategory)
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
        , class_<XSpriteItem, bases<DatabaseItem> >("XSpriteItem")
            .def("addAnim", &XSpriteItem::addAnim)
        // Tile
        , class_<Tile, bases<XSpriteItem> >("Tile")
            .def(constructor<const std::string&>())
            .def(constructor<const std::string&, const bool&>())
            .def("setProtection", &Tile::setProtection)
            .def("isOrientable", &Tile::isOrientable)
            .def("protection", &Tile::protection)
        // Unit
        , class_<Unit, bases<XSpriteItem> >("Unit")
            .def(constructor<const std::string&>())
        // Building
        , class_<Building, bases<XSpriteItem> >("Building")
            .def(constructor<const std::string&, const int&, const bool&>())
            .def("setResistance", &Building::setResistance)
            .def("addProduction", &Building::addProduction)
            .def("resistance", &Building::setResistance)
            .def("capturable", &Building::capturable)
        // Category
        , class_<Category, bases<DatabaseItem> >("Category")
            .def(constructor<const std::string&>())
            .def("addItem", &Category::addItem)
            .def("isItemIn", &Category::isItemIn)
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
                .def(constructor<sf::Vector2i>())
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
            .def("getTileType", (std::string(Map::*)(const unsigned int&,
                const unsigned int&)const)&Map::getTileType)
            .def("getTileType", (std::string(Map::*)(const sf::Vector2i&)const)
                &Map::getTileType)
            .def("setTile", (void(Map::*)(const unsigned int&,
                const unsigned int&, const std::string &))&Map::setTile)
            .def("setTile", (void(Map::*)(const sf::Vector2i&,
                const std::string&))&Map::setTile)
            .def("setTileAnim", (void(Map::*)(const unsigned int&,
                const unsigned int&, const std::string &))&Map::setTileAnim)
            .def("setTileAnim", (void(Map::*)(const sf::Vector2i&,
                const std::string&))&Map::setTileAnim)
            .def("getTilePtr", (GameEntity*(Map::*)
                (const unsigned int&,const unsigned int&))
                 &Map::getTilePtr)
            .def("getTilePtr", (GameEntity*(Map::*)
                 (const sf::Vector2i&))&Map::getTilePtr)
             .def("getTileConstPtr", (const GameEntity*(Map::*)
                (const unsigned int&,const unsigned int&)const)
                 &Map::getTileConstPtr)
            .def("getTileConstPtr", (const GameEntity*(Map::*)
                 (const sf::Vector2i&)const)&Map::getTileConstPtr)
            .def("getTileOrientation", (Orientation(Map::*)(const unsigned int&,
                const unsigned int&)const)&Map::getTileOrientation)
            .def("getTileOrientation", (Orientation(Map::*)(const sf::Vector2i&)
                const)&Map::getTileOrientation)
            .def("isInsideMap", (bool(Map::*)(const unsigned int&,
                const unsigned int&)const)&Map::isInsideMap)
            .def("isInsideMap", (bool(Map::*)(const sf::Vector2i&)const)
                &Map::isInsideMap)
            // XSprite
            , class_<XSprite>("XSprite")
                .def("playAnim", (void(XSprite::*)(const db::Animation *anim))
                    &XSprite::playAnim)
                .def("setFrame", &XSprite::setFrame)
                .def("pauseAnim", &XSprite::pauseAnim)
                .def("stopAnim", &XSprite::stopAnim)
                .def("startAnim", &XSprite::startAnim)
                .def("restartAnim", &XSprite::startAnim)
                .def("isAnimPaused", &XSprite::isAnimPaused)
                .def("isAnimStopped", &XSprite::isAnimStopped)
                .def("currentFrame", &XSprite::currentFrame)
            // GameEntity
            , class_<GameEntity>("GameEntity")
                .def("setPosition", (void(GameEntity::*)(const sf::Vector2i&))
                    &GameEntity::setPosition)
                .def("setPosition", (void(GameEntity::*)(const int&, const int&))
                    &GameEntity::setPosition)
                .def("updatePosition", &GameEntity::updatePosition)
                .def("playAnim", (void(GameEntity::*)(const std::string&))
                    &GameEntity::playAnim)
                .def("playAnim", (void(GameEntity::*)(const std::string&,
                    const bool&))&GameEntity::playAnim)
                .def("type", &GameEntity::type)
                .def("alias", &GameEntity::alias)
                .def("position", &GameEntity::position)
                .def("orientation", &GameEntity::orientation)
                .def("setOrientation", &GameEntity::setOrientation)
                .def("xsprite", &GameEntity::xsprite)
                .def("xspriteConst", &GameEntity::xspriteConst)
                .enum_("Orientation") // ENUM - Orientation
                [
                    value("UNDEFINED", 0),
                    value("RIGHT", 1),
                    value("LEFT", 2),
                    value("UPWARD", 3),
                    value("DOWNWARD", 4)
                ]
    ];
}

#endif /* DB_EXPORTER */
