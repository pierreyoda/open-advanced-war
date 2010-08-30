#include <SFML/Graphics.hpp>
#include <iostream>
#include "Map.hpp"
#include "lua/LuaVirtualMachine.hpp"

using namespace sf;

Map::Map(const sf::Vector2ui &size)
{
    for (unsigned int i = 0; i < size.y; i++)
    {
        m_tiles.push_back(std::vector<GameEntity*>());
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
    for (unsigned int i = 0; i < m_tiles.size(); i++)
        for (unsigned int j = 0; j < m_tiles[i].size(); j++)
            delete m_tiles[i][j];
    m_tiles.clear();
}

void Map::renderTo(RenderTarget &target)
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
}

std::string Map::getTileType(const Vector2i &pos) const
{
    return getTileType(pos.x, pos.y);
}
std::string Map::getTileType(const unsigned int &x, const unsigned int &y)
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

void Map::setTile(const Vector2i &pos, const std::string &type)
{
    return setTile(pos.x, pos.y, type);
}
void Map::setTile(const unsigned int &x, const unsigned int &y,
    const std::string &type)
{
    if (!isInsideMap(x,y))
        return;
    GameEntity *tile = new GameEntity(type);
    tile->setPosition(x, y);
    tile->playAnim("base", true);
    try
    {
        luabind::call_function<void>(LuaVM::getInstance().getLua(), // Calling lua function "onTilePlaced"
            "onTilePlaced",
            tile,
            this);
    }
    catch (const std::exception &exception)
    {
        std::cerr << lua_tostring(LuaVM::getInstance().getLua(), -1)<< "\n";
    }
    m_tiles[y][x] = tile;
}

void Map::setTileAnim(const Vector2i &pos, const std::string &anim)
{
    return setTile(pos.x, pos.y, anim);
}
void Map::setTileAnim(const unsigned int &x, const unsigned int &y,
    const std::string &anim)
{
    if (!isInsideMap(x,y))
        return;
    GameEntity *ptr = getTilePtr(x, y);
    if (ptr != 0)
        ptr->playAnim(anim, true);
}

void Map::setTileOrientation(const sf::Vector2i &pos, const std::string &type,
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
    try
    {
        luabind::call_function<void>(LuaVM::getInstance().getLua(), // Calling lua function "onTilePlaced"
            "onTileOriented",
            ptr,
            this);
    }
    catch (const std::exception &exception)
    {
        std::cerr << lua_tostring(LuaVM::getInstance().getLua(), -1)<< "\n";
    }
}

bool Map::isInsideMap(const sf::Vector2i &pos) const
{
    return isInsideMap(pos.x, pos.y);
}
bool Map::isInsideMap(const unsigned int &x, const unsigned int &y) const
{
    return (x >= 0 && y >= 0 && y < m_tiles.size() && x < m_tiles[y].size());
}
