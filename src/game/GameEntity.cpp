#include "GameEntity.hpp"
#include "../GameGlobals.hpp"

GameEntity::GameEntity(const std::string &name) : m_name(name),
    m_orientation(RIGHT)
{

}

GameEntity::~GameEntity()
{

}

void GameEntity::setPosition(const sf::Vector2i &pos)
{
    setPosition(pos.x, pos.y);
}
void GameEntity::setPosition(const int &x, const int &y)
{
    SetPosition(x * gg->case_w, y * gg->case_h);
}
