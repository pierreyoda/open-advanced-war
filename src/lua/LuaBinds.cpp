#include "LuaBinds.hpp"
#include "../db/Database.hpp"

#ifndef DB_EXPORTER
    #define db_member def_readonly // Read-only access
    #include "../Map.hpp"
    #include "../Game.hpp"
    #include "../gui/GuiSpace.hpp"
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
/* A macro that simplifitates operator declaration.
* Ex : operator+(int) --> OPERATOR(+, int())
*/
#define OPERATOR(operator, otherType) .def(self operator otherType)
#define OPERATOR_CONST(operator, otherType) .def(const_self operator otherType)

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

namespace sf
{
    class KeyBind
    { };
}

void exportInputAndEvents(lua_State *lua) /// Code from sfengine (http://code.google.com/p/sfengine/)
{
    module(lua, "sf")
    [
        // sf::Key::Code
        class_<sf::KeyBind>("Key")
            .enum_("Code")
            [
                value("A", 'a'),
                value("B", 'b'),
                value("C", 'c'),
                value("D", 'd'),
                value("E", 'e'),
                value("F", 'f'),
                value("G", 'g'),
                value("H", 'h'),
                value("I", 'i'),
                value("J", 'j'),
                value("K", 'k'),
                value("L", 'l'),
                value("M", 'm'),
                value("N", 'n'),
                value("O", 'o'),
                value("P", 'p'),
                value("Q", 'q'),
                value("R", 'r'),
                value("S", 's'),
                value("T", 't'),
                value("U", 'u'),
                value("V", 'v'),
                value("W", 'w'),
                value("X", 'x'),
                value("Y", 'y'),
                value("Z", 'z'),
                value("Num0", '0'),
                value("Num1", '1'),
                value("Num2", '2'),
                value("Num3", '3'),
                value("Num4", '4'),
                value("Num5", '5'),
                value("Num6", '6'),
                value("Num7", '7'),
                value("Num8", '8'),
                value("Num9", '9'),
                value("Escape", 256),
                value("LControl", 257),
                value("LShift", 258),
                value("LAlt", 259),
                value("LSystem", 260),
                value("RControl", 261),
                value("RShift", 262),
                value("RAlt", 263),
                value("RSystem", 264),
                value("Menu", 265),
                value("LBracket", 266),
                value("RBracket", 267),
                value("SemiColon", 268),
                value("Coma", 269),
                value("Period", 270),
                value("Quote", 271),
                value("Slash", 272),
                value("BackSlash", 273),
                value("Tilde", 274),
                value("Equal", 275),
                value("Dash", 276),
                value("Space", 277),
                value("Return", 278),
                value("Back", 279),
                value("Tab", 280),
                value("PageUp", 281),
                value("PageDown", 282),
                value("End", 283),
                value("Home", 284),
                value("Insert", 285),
                value("Delete", 286),
                value("Add", 287),
                value("Subtract", 288),
                value("Multiply", 289),
                value("Divide", 290),
                value("Left", 291),
                value("Right", 292),
                value("Up", 293),
                value("Down", 294),
                value("Numpad0", 295),
                value("Numpad1", 296),
                value("Numpad2", 297),
                value("Numpad3", 298),
                value("Numpad4", 299),
                value("Numpad5", 300),
                value("Numpad6", 301),
                value("Numpad7", 302),
                value("Numpad8", 303),
                value("Numpad9", 304),
                value("F1", 305),
                value("F2", 306),
                value("F3", 307),
                value("F4", 308),
                value("F5", 309),
                value("F6", 310),
                value("F7", 311),
                value("F8", 312),
                value("F9", 313),
                value("F10", 314),
                value("F11", 315),
                value("F12", 316),
                value("F13", 317),
                value("F14", 318),
                value("F15", 319),
                value("Pause", 320),
                value("Count", 321)
            ]
        // sf::Event
        , class_<sf::Event>("Event")
            .def_readonly("Type", &sf::Event::Type)
            .enum_("EventType")
            [
                /*value("Closed", 0),
                value("Resized", 1),
                value("LostFocus", 2),
                value("GainedFocus", 3),
                value("TextEntered", 4),*/
                value("KeyPressed", 5),
                value("KeyReleased", 6),
                value("MouseWheelMoved", 7),
                value("MousePressed", 8),
                value("MouseReleased", 9),
                value("MouseMoved", 10),
                value("MouseEntered", 11)/*,
                value("MouseLeft", 12)*/
            ]
            .def_readonly("Key", &sf::Event::Key)
            .def_readonly("Mouse", &sf::Event::MouseMove)
            .def_readonly("MouseButton", &sf::Event::MouseButton)
        // sf::KeyEvent
        , class_<sf::Event::KeyEvent>("KeyEvent")
                .def_readonly("Code", &sf::Event::KeyEvent::Code)
                .def_readonly("Alt", &sf::Event::KeyEvent::Alt)
                .def_readonly("Control", &sf::Event::KeyEvent::Control)
                .def_readonly("Shit", &sf::Event::KeyEvent::Shift)
        // sf::Event::MouseMoveEvent
        , class_<sf::Event::MouseMoveEvent>("MouseMoveEvent")
            .def_readonly("X", &sf::Event::MouseMoveEvent::X)
            .def_readonly("Y", &sf::Event::MouseMoveEvent::Y)
        // sf::Event::MouseButtonEvent
        , class_<sf::Event::MouseButtonEvent>("MouseButtonEvent")
            .def_readonly("Button", &sf::Event::MouseButtonEvent::Button)
            .def_readonly("X", &sf::Event::MouseButtonEvent::X)
            .def_readonly("Y", &sf::Event::MouseButtonEvent::Y)
        // sf::Mouse::Button
        , class_<sf::Mouse::Button>("Mouse")
            .enum_("Button")
            [
                value("Left", 0),
                value("Right", 1),
                value("Middle", 2),
                value("XButton1", 3),
                value("XButton2", 4)/*,
                value("ButtonCount", 5)*/
            ]
        , class_<sf::Input>("Input")
            .def(luabind::constructor<>())
            .def("IsKeyDown", &sf::Input::IsKeyDown)
            .def("GetMouseX", &sf::Input::GetMouseX)
            .def("GetMouseY", &sf::Input::GetMouseY)
            .def("IsMouseButtonDown", &sf::Input::IsMouseButtonDown)
    ];
}

void LuaBinds::exportTools(lua_State *lua)
{
    module(lua)
    [
        namespace_("sf")
        [
            // sf::Vector2i (sf::Vector2<int>)
            class_<sf::Vector2i>("Vector2i")
                .def(constructor< >())
                .def(constructor<int, int>())
                .def(constructor<sf::Vector2i>())
                .def_readwrite("x", &sf::Vector2i::x)
                .def_readwrite("y", &sf::Vector2i::y)
                OPERATOR(+, sf::Vector2i())
                OPERATOR(-, sf::Vector2i())
                OPERATOR(/, int())
                OPERATOR(*, int())
                OPERATOR_CONST(==, sf::Vector2i())
            // sf::Vector2f (sf::Vector2<float>)
            , class_<sf::Vector2f>("Vector2f")
                .def(constructor< >())
                .def(constructor<float, float>())
                .def(constructor<sf::Vector2f>())
                .def_readwrite("x", &sf::Vector2f::x)
                .def_readwrite("y", &sf::Vector2f::y)
                OPERATOR(+, sf::Vector2f())
                OPERATOR(-, sf::Vector2f())
                OPERATOR(/, float())
                OPERATOR(*, float())
                OPERATOR_CONST(==, sf::Vector2f())
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
    exportInputAndEvents(lua);
}

void LuaBinds::exportGame(lua_State *lua)
{
    using namespace luabind;
    module(lua)
    [
        // Game
        class_<Game>("Game")
            .def("addGui", (void(Game::*)(const std::string&))&Game::addGui)
            .def("addGui", (void(Game::*)(const std::string&, const bool&))
                 &Game::addGui)
            DEF(Game, currentGui)
            DEF(Game, setCurrentGui)
            DEF(Game, guiExists)
            DEF(Game, startDrawingXSprite)
            DEF(Game, stopDrawingXSprite)
            DEF(Game, getChoiceFromTable)
            DEF(Game, spawnUnit)
            DEF(Game, isUnitPresent)
            DEF(Game, getGlobalAffector)
            DEF(Game, setGlobalAffector)
            DEF(Game, getMapPtr)
            DEF(Game, newMap)
            DEF(Game, saveMap)
            DEF(Game, loadMap)
            DEF(Game, addArmy)
            DEF(Game, getArmy)
            DEF(Game, nbOfArmies)
        // ArmyGeneral
        , class_<ArmyGeneral>("ArmyGeneral")
            DEF(ArmyGeneral, id)
            DEF(ArmyGeneral, getUnitId)
            DEF(ArmyGeneral, getUnitPtr)
            DEF(ArmyGeneral, addUnit)
            DEF(ArmyGeneral, removeUnit)
        // Map
        , class_<Map>("Map")
            DEF(Map, size)
            // Building
            .def("placeBuilding", (bool(Map::*)(const unsigned int&,
                const unsigned int&, const std::string&, const std::string&,
                const bool&))&Map::placeBuilding)
            .def("placeBuilding", (bool(Map::*)(const sf::Vector2i&,
                const std::string&, const std::string&, const bool&))
                 &Map::placeBuilding)
            DEF(Map, removeBuilding)
            DEF(Map, isBuildingPresent)
            DEF(Map, getBuilding)
            DEF(Map, getBuildingType)
            DEF(Map, findBuildingPos)
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
            DEF(XSprite, currentAnimPlayed)
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
            .def("faction", &GameEntity::faction)
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
            DEF(Unit, id)
        // GUI
        , class_<GuiSpace>("GuiSpace")
            DEF(GuiSpace, addButton)
            DEF(GuiSpace, addVerticalSpriteList)
            DEF(GuiSpace, name)
        , class_<VerticalSpriteItemList>("VerticalSpriteItemList")
            DEF(VerticalSpriteItemList, addItem)
    ];
}

#endif /* DB_EXPORTER */
