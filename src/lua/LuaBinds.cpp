#include "LuaBinds.hpp"
#include "../db/Database.hpp"

#ifndef DB_EXPORTER
    #define db_member def_readonly // Read-only access
    #include "../Map.hpp"
    #include "../Game.hpp"
    #include "../gui/EditorGui.hpp"
    #include "../game/Unit.hpp"
    #include "../game/ArmyGeneral.hpp"
    #include "../tools/PausableClock.hpp"
    #include "../tools/FilesPathHandler.hpp"
#endif /* DB_EXPORTER */

/* A macro that simplifitates method declaration (NB : C++ name will become Lua name).
* name Method's C++ (and Lua) name
* class Class name.
*/
#define DEF(class, method) .def(BOOST_PP_STRINGIZE(method), &class::method)

using namespace luabind;

void LuaBinds::exportDatabase(lua_State *lua)
{
    using namespace db;

    module(lua, "db")
    [
        // DatabaseItem
        class_<DatabaseItem>("DatabaseItem")
            .def("name", &DatabaseItem::name)
        // Database
        , class_<Database>("Database")
            DEF(Database, getModuleName)
            DEF(Database, addTile)
            DEF(Database, addUnit)
            DEF(Database, addBuilding)
            DEF(Database, addCategory)
            DEF(Database, addFaction)
            DEF(Database, addPropulsion)
            DEF(Database, addWeapon)
            DEF(Database, findTile)
            DEF(Database, findUnit)
            DEF(Database, findBuilding)
            DEF(Database, findFaction)
            DEF(Database, findWeapon)
            DEF(Database, findPropulsion)
            DEF(Database, findCategory)
        // TranslationProvider
        , class_<TranslationProvider>("TranslationProvider")
            .def("tr", (std::string(TranslationProvider::*)(const std::string&))
                &TranslationProvider::tr)
            .def("tr", (std::string(TranslationProvider::*)(const std::string&,
                    const bool&))&TranslationProvider::tr)
            DEF(TranslationProvider, selectLang)
            DEF(TranslationProvider, translateItem)
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
            .def("addFrame", (Animation&(Animation::*)(const Frame&))
                &Animation::addFrame)
            .def("addFrame", (Animation&(Animation::*)(const unsigned int &,
                const unsigned int&))&Animation::addFrame)
            .def("addFrame", (Animation&(Animation::*)(const unsigned int &,
                const unsigned int&, const float&))&Animation::addFrame)
            DEF(Animation, setImage)
            DEF(Animation, clear)
            DEF(Animation, image)
            DEF(Animation, getFrame)
        // XSpriteItem
        , class_<XSpriteItem, bases<DatabaseItem> >("XSpriteItem")
            .def("addAnim", (XSpriteItem&(XSpriteItem::*)(const Animation&))
                &XSpriteItem::addAnim)
            .def("addAnim", (XSpriteItem&(XSpriteItem::*)(const Animation&,
                const std::string&))&XSpriteItem::addAnim)
            DEF(XSpriteItem, findAnim)
        // Tile
        , class_<Tile, bases<XSpriteItem> >("Tile")
            .def(constructor<const std::string&>())
            .def(constructor<const std::string&, const bool&>())
            DEF(Tile, setProtection)
            DEF(Tile, isOrientable)
            DEF(Tile, protection)
        // Unit
        , class_<db::Unit, bases<XSpriteItem> >("Unit")
            .def(constructor<const std::string&>())
            DEF(db::Unit, addIntCaracteristic)
            DEF(db::Unit, addBoolCaracteristic)
            DEF(db::Unit, findIntCaracteristic)
            DEF(db::Unit, findBoolCaracteristic)
            DEF(db::Unit, propulsion)
            DEF(db::Unit, setPropulsion)
            .enum_("Tribool")
            [
                value("INDETERMINATE", 2),
                value("TRUE", 1),
                value("FALSE", 0)
            ]
        // Building
        , class_<Building, bases<XSpriteItem> >("Building")
            .def(constructor<const std::string&, const int&, const bool&>())
            DEF(Building, setResistance)
            DEF(Building, addProduction)
            DEF(Building, resistance)
            DEF(Building, capturable)
        // Category
        , class_<Category, bases<DatabaseItem> >("Category")
            .def(constructor<const std::string&>())
            DEF(Category, addItem)
            DEF(Category, isItemIn)
        // Propulsion
        , class_<Propulsion, bases<DatabaseItem> >("Propulsion")
            .def(constructor<const std::string&>())
            DEF(Propulsion, canMoveTo)
            DEF(Propulsion, moveCost)
            DEF(Propulsion, addCanMoveTo)
        // Faction
        , class_<Faction, bases<DatabaseItem> >("Faction")
            .def(constructor<const std::string&>())
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
            // sf::Vector2f (sf::Vector2<float>)
            , class_<sf::Vector2f>("Vector2f")
                .def(constructor< >())
                .def(constructor<float, float>())
                .def(constructor<sf::Vector2f>())
                .def_readwrite("x", &sf::Vector2f::x)
                .def_readwrite("y", &sf::Vector2f::y)
            // sf::Color
            , class_<sf::Color>("Color")
                .def(constructor< >())
                .def(constructor<sf::Uint8, sf::Uint8, sf::Uint8>())
                .def(constructor<sf::Uint8, sf::Uint8, sf::Uint8, sf::Uint8>())
                .def_readwrite("r", &sf::Color::r)
                .def_readwrite("g", &sf::Color::g)
                .def_readwrite("b", &sf::Color::b)
                .def_readwrite("a", &sf::Color::a)
            // sf::IntRect (sf::Rect<int>)
            , class_<sf::IntRect>("IntRect")
                .def(constructor< >())
                .def(constructor<int, int, int, int>())
                .def(constructor<sf::IntRect>())
                .def_readwrite("x", &sf::IntRect::Left)
                .def_readwrite("y", &sf::IntRect::Top)
                .def_readwrite("w", &sf::IntRect::Width)
                .def_readwrite("h", &sf::IntRect::Height)
            // sf::FloatRect (sf::Rect<float>)
            , class_<sf::FloatRect>("FloatRect")
                .def(constructor< >())
                .def(constructor<float, float, float, float>())
                .def(constructor<sf::FloatRect>())
                .def_readwrite("x", &sf::FloatRect::Left)
                .def_readwrite("y", &sf::FloatRect::Top)
                .def_readwrite("w", &sf::FloatRect::Width)
                .def_readwrite("h", &sf::FloatRect::Height)
        ]
        // PausableClock
        , class_<PausableClock>("PausableClock")
            .def(constructor< >())
            .def(constructor<bool>())
            DEF(PausableClock, pause)
            DEF(PausableClock, start)
            DEF(PausableClock, getElapsedTime)
            .def("reset", (void(PausableClock::*)())&PausableClock::reset)
            .def("reset", (void(PausableClock::*)(const bool&))&PausableClock::reset)
        // FilesPathHandler
        , class_<FilesPathHandler>("FilesPathHandler")
            .def("addFile", (void(FilesPathHandler::*)(const std::string&,
                const std::string&))&FilesPathHandler::addFile)
            .def("addFile", (void(FilesPathHandler::*)(const std::string&,
                const std::string&, const bool&))&FilesPathHandler::addFile)
            DEF(FilesPathHandler, getFilepath)
            DEF(FilesPathHandler, scanDirectory)
    ];
}

void LuaBinds::exportGame(lua_State *lua)
{
    using namespace luabind;
    module(lua)
    [
        // Game
        class_<Game>("Game")
            DEF(Game, startDrawingXSprite)
            DEF(Game, stopDrawingXSprite)
            DEF(Game, spawnUnit)
            DEF(Game, isUnitPresent)
            DEF(Game, getGlobalAffector)
            DEF(Game, setGlobalAffector)
            DEF(Game, getMapPtr)
            DEF(Game, setEditorTile)
            DEF(Game, setEditorBuilding)
            DEF(Game, setEditorUnit)
            DEF(Game, setEditorFaction)
        // ArmyGeneral
        , class_<ArmyGeneral>("ArmyGeneral")
            DEF(ArmyGeneral, id)
            DEF(ArmyGeneral, getUnitId)
        // Map
        , class_<Map>("Map")
            // Building
            .def("placeBuilding", (void(Map::*)(const unsigned int&,
                const unsigned int&, const std::string&, const std::string&,
                const bool&))&Map::placeBuilding)
            .def("placeBuilding", (void(Map::*)(const sf::Vector2i&,
                const std::string&, const std::string&, const bool&))
                 &Map::placeBuilding)
            DEF(Map, removeBuilding)
            DEF(Map, isBuildingPresent)
            DEF(Map, getBuildingType)
            // Tiles - modifiers
            .def("setTile", (void(Map::*)(const unsigned int&,
                const unsigned int&, const std::string &))&Map::setTile)
            .def("setTile", (void(Map::*)(const sf::Vector2i&,
                const std::string&))&Map::setTile)
            .def("setTileAnim", (void(Map::*)(const unsigned int&,
                const unsigned int&, const std::string &))&Map::setTileAnim)
            .def("setTileAnim", (void(Map::*)(const sf::Vector2i&,
                const std::string&))&Map::setTileAnim)
            // Tiles - accessors
            .def("getTileType", (std::string(Map::*)(const unsigned int&,
                const unsigned int&)const)&Map::getTileType)
            .def("getTileType", (std::string(Map::*)(const sf::Vector2i&)const)
                &Map::getTileType)
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
            // General functions
            .def("isInsideMap", (bool(Map::*)(const unsigned int&,
                const unsigned int&)const)&Map::isInsideMap)
            .def("isInsideMap", (bool(Map::*)(const sf::Vector2i&)const)
                &Map::isInsideMap)
        // XSprite
        , class_<XSprite>("XSprite")
            .def(constructor< >())
            .def("playAnim", (void(XSprite::*)(const db::Animation *anim))
                &XSprite::playAnim)
            .def("playAnim", (void(XSprite::*)(const db::Animation *anim,
               const bool&))&XSprite::playAnim)
            DEF(XSprite, setFrame)
            DEF(XSprite, pauseAnim)
            DEF(XSprite, stopAnim)
            DEF(XSprite, startAnim)
            DEF(XSprite, restartAnim)
            .def("setFilter", (void(XSprite::*)(const sf::Color&))
                &XSprite::setFilter)
            .def("setFilter", (void(XSprite::*)(const sf::Color&,
                const bool&))&XSprite::setFilter)
            DEF(XSprite, useFilter)
            DEF(XSprite, isAnimPaused)
            DEF(XSprite, isAnimStopped)
            DEF(XSprite, isFilterUsed)
            DEF(XSprite, currentFilter)
            DEF(XSprite, currentFrame)
            .def("SetPosition", (void(XSprite::*)(float, float))
                &XSprite::SetPosition)
            .def("SetPosition", (void(XSprite::*)(const sf::Vector2f&))
                &XSprite::SetPosition)
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
            .def("getClass", &GameEntity::getClass)
            .def("type", &GameEntity::type)
            .def("alias", &GameEntity::alias)
            .def("position", &GameEntity::position)
            .def("ownerId", &GameEntity::ownerId)
            .def("orientation", &GameEntity::orientation)
            .def("setOrientation", &GameEntity::setOrientation)
            .def("xsprite", &GameEntity::xsprite)
            .def("xspriteConst", &GameEntity::xspriteConst)
            // Caracteristics
            .def("setIntCaracteristic", &GameEntity::setIntCaracteristic)
            .def("setBoolCaracteristic", &GameEntity::setIntCaracteristic)
            .def("setStringCaracteristic", &GameEntity::setIntCaracteristic)
            .enum_("Orientation") // ENUM - Orientation
            [
                value("UNDEFINED", 0),
                value("RIGHT", 1),
                value("LEFT", 2),
                value("UPWARD", 3),
                value("DOWNWARD", 4)
            ]
            .enum_("Classes") // ENUM - Classes
            [
                value("NONE", 0),
                value("TILE", 1),
                value("BUILDING", 2),
                value("UNIT", 3)
            ]
            .scope // Static functions
            [
                def("findClassFromType", &GameEntity::findClassFromType),
                def("pixelsToTiles", (sf::Vector2i(*)(const sf::Vector2i&))
                    &GameEntity::pixelsToTiles),
                def("pixelsToTiles", (sf::Vector2i(*)(const sf::Vector2f&))
                    &GameEntity::pixelsToTiles),
                def("tilesToPixels", (sf::Vector2f(*)(const sf::Vector2i&))
                    &GameEntity::tilesToPixels)
            ]
        // GameEntity - Unit (Lua : GameUnit)
        , class_<Unit, bases<GameEntity> >("GameUnit")
            .def("id", &Unit::id)
        // GUI
        , class_<EditorGui>("EditorGui")
            DEF(EditorGui, addVerticalSpriteList)
        , class_<VerticalSpriteItemList>("VerticalSpriteItemList")
            DEF(VerticalSpriteItemList, addItem)
    ];
}

#endif /* DB_EXPORTER */
