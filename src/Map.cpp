#include <SFML/Graphics.hpp>
#include "Map.hpp"

using namespace sf;

Map::Map(const sf::Vector2ui &size)
{
    for (unsigned int i = 0; i < size.y; i++)
    {
        m_tiles.push_back(std::vector<GameEntity*>());
        for (unsigned int j = 0; j < size.x; j++)
        {
            GameEntity *tile = new GameEntity("plain");
            tile->setPosition(j, i);
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
            ptr->update();
            target.Draw(*ptr);
        }
}

const std::string Map::getTile(const Vector2ui &pos) const
{
    return getTile(pos.x, pos.y);
}
const std::string Map::getTile(const unsigned int &x, const unsigned int &y)
    const
{
    return "";
}

void Map::setTile(const Vector2ui &pos, const std::string &type)
{
    return setTile(pos.x, pos.y, type);
}
void Map::setTile(const unsigned int &x, const unsigned int &y,
    const std::string &type)
{

}
