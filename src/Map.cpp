#include <iostream>
#include <SFML/Graphics.hpp>
#include <luabind/adopt_policy.hpp>
#include "Map.hpp"
#include "lua/LuaVirtualMachine.hpp"

using namespace std;

Map::Map(const sf::Vector2ui &size) : m_prevMouseOver(0)
{
    for (unsigned int i = 0; i < size.y; i++)
    {
        m_tiles.push_back(vector<GameEntity*>());
        for (unsigned int j = 0; j < size.x; j++)
        {
            GameEntity *tile = new GameEntity("Plain");
            tile->setPosition(j, i);
            tile->playAnim("base");
            m_tiles[i].push_back(tile);
        }
    }
}

Map::~Map()
{
    m_prevMouseOver = 0;
    delete m_prevMouseOver;
    for (unsigned int i = 0; i < m_tiles.size(); i++)
        for (unsigned int j = 0; j < m_tiles[i].size(); j++)
            delete m_tiles[i][j];
    m_tiles.clear();
    for (std::list<GameEntity*>::iterator iter = m_buildings.begin();
        iter != m_buildings.end(); iter++)
        delete *iter;
    m_buildings.clear();
}

void Map::renderTo(sf::RenderTarget &target)
{
    for (unsigned int i = 0; i < m_tiles.size(); i++)
        for (unsigned int j = 0; j < m_tiles[i].size(); j++)
        {
            GameEntity *ptr = m_tiles[i][j];
            if (ptr == 0)
                continue;
            ptr->xsprite().update();
            target.Draw(ptr->xspriteConst());
        }
    for (list<GameEntity*>::iterator iter = m_buildings.begin();
        iter != m_buildings.end(); iter++)
    {
            if (*iter == 0)
                continue;
            (*iter)->xsprite().update();
            target.Draw((*iter)->xspriteConst());
    }
}

/// TODO (Pierre-Yves#3#): [SCRIPTING] Simplifiate lua function calling
void Map::onMouseOver(const sf::Vector2i &tilePos)
{
    if (m_prevMouseOver != 0 && m_prevMouseOver->position() == tilePos)
        return; // Already selected
    GameEntity *ptr = 0;
    bool stop = false;
    for (list<GameEntity*>::iterator iter = m_buildings.begin();
        iter != m_buildings.end(); iter++)
    {
        if (*iter == 0)
            continue;
        if ((*iter)->position() == tilePos)
        {
            ptr = (*iter);
            break;
        }
    }
    if (ptr == 0) // Still not found...
    {
        for (unsigned int i = 0; i < m_tiles.size(); i++)
        {
            for (unsigned int j = 0; j < m_tiles[i].size(); j++)
            {
                if (m_tiles[i][j] != 0 && m_tiles[i][j]->position() == tilePos)
                {
                    ptr = m_tiles[i][j];
                    stop = true;
                    break;
                }
            }
            if (stop)
                break;
        }
    }
    if (ptr == 0) // Not found : exiting
        return;
    try // Calling lua function "onMouseNoMoreOverGameEntity"
    {
        luabind::call_function<void>(LuaVM::getInstance().getLua(),
            "onMouseNoMoreOverGameEntity",
            m_prevMouseOver);
    }
    catch (const exception &exception)
    {
        cerr << lua_tostring(LuaVM::getInstance().getLua(), -1) << "\n";
        m_prevMouseOver = 0;
    }
    static bool luaError = false, luaError2 = false;
    if (!luaError)
        CALL_LUA_FUNCTION(LuaVM::getInstance().getLua(), void,
            "onMouseNoMoreOverGameEntity", luaError, m_prevMouseOver);
    m_prevMouseOver = ptr;
    if (!luaError2)
        CALL_LUA_FUNCTION(LuaVM::getInstance().getLua(), void,
            "onMouseOverGameEntity", luaError2, ptr)
}

void Map::placeBuilding(const sf::Vector2i &pos, const string &type,
    const bool &force)
{
    bool ok = false;
    static bool luaError = false;
    if (!luaError)
        CALL_LUA_RFUNCTION(LuaVM::getInstance().getLua(), bool, ok,
           "canPlaceBuilding", luaError, type, pos, this, force)
    if (!ok)
        return;
    GameEntity *building = new GameEntity(type);
        building->setPosition(pos);
        building->playAnim("base", true);
    m_buildings.push_back(building);
}
void Map::placeBuilding(const unsigned int &x, const unsigned int &y,
    const string &type, const bool &force)
{
    placeBuilding(sf::Vector2i(x, y), type, force);
}

string Map::getTileType(const sf::Vector2i &pos) const
{
    return getTileType(pos.x, pos.y);
}
string Map::getTileType(const unsigned int &x, const unsigned int &y)
    const
{
    const GameEntity *ptr = getTileConstPtr(x, y);
    if (ptr == 0)
        return "";
    return ptr->type();
}

Orientation Map::getTileOrientation(const sf::Vector2i &pos) const
{
    return getTileOrientation(pos.x, pos.y);
}
Orientation Map::getTileOrientation(const unsigned int &x,
    const unsigned int &y) const
{
    const GameEntity *ptr = getTileConstPtr(x, y);
    if (ptr != 0)
        return ptr->orientation();
    return UNDEFINED;
}

GameEntity *Map::getTilePtr(const sf::Vector2i &pos)
{
    return getTilePtr(pos.x, pos.y);
}
GameEntity *Map::getTilePtr(const unsigned int &x, const unsigned int &y)
{
    if (!isInsideMap(x, y))
        return 0;
    return m_tiles[y][x];
}

const GameEntity *Map::getTileConstPtr(const sf::Vector2i &pos) const
{
    return getTileConstPtr(pos.x, pos.y);
}
const GameEntity *Map::getTileConstPtr(const unsigned int &x,
    const unsigned int &y) const
{
    if (!isInsideMap(x, y))
        return 0;
    return m_tiles[y][x];
}

void Map::setTile(const sf::Vector2i &pos, const string &type)
{
    return setTile(pos.x, pos.y, type);
}
void Map::setTile(const unsigned int &x, const unsigned int &y,
    const string &type)
{
    if (!isInsideMap(x,y))
        return;
    GameEntity *tile = new GameEntity(type);
    tile->setPosition(x, y);
    tile->playAnim("base", true);
    static bool luaError = false;
    if (!luaError)
        CALL_LUA_FUNCTION(LuaVM::getInstance().getLua(), void, "onTilePlaced",
            luaError, tile, this)
    m_tiles[y][x] = tile;
}

void Map::setTileAnim(const sf::Vector2i &pos, const string &anim)
{
    return setTileAnim(pos.x, pos.y, anim);
}
void Map::setTileAnim(const unsigned int &x, const unsigned int &y,
    const string &anim)
{
    if (!isInsideMap(x,y))
        return;
    GameEntity *ptr = getTilePtr(x, y);
    if (ptr != 0)
        ptr->playAnim(anim, true);
}

void Map::setTileOrientation(const sf::Vector2i &pos, const string &type,
    const Orientation &orientation)
{
    setTileOrientation(pos.x, pos.y, orientation);
}
void Map::setTileOrientation(const unsigned int &x, const unsigned int &y,
    const Orientation &orientation)
{
    GameEntity *ptr = getTilePtr(x, y);
    if (ptr == 0)
        return;
    ptr->setOrientation(orientation);
    static bool luaError = false;
    if (!luaError)
        CALL_LUA_FUNCTION(LuaVM::getInstance().getLua(), void, "onTileOriented",
            luaError, ptr, this)
}

bool Map::isInsideMap(const sf::Vector2i &pos) const
{
    return isInsideMap(pos.x, pos.y);
}
bool Map::isInsideMap(const unsigned int &x, const unsigned int &y) const
{
    return (x >= 0 && y >= 0 && y < m_tiles.size() && x < m_tiles[y].size());
}
