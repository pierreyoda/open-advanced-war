#include "GameEntity.hpp"
#include "../GameGlobals.hpp"
#include "../db/Database.hpp"

GameEntity::GameEntity(const std::string &name) : m_pos(0, 0), m_type(name),
    m_alias(/*TODO*/), m_orientation(RIGHT)
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
    m_pos.x = x, m_pos.y = y;
    updatePosition();
}

void GameEntity::updatePosition()
{
    m_xsprite.SetPosition(m_pos.x * gg.case_w, m_pos.y * gg.case_h);
}

void GameEntity::playAnim(const std::string &anim, const bool &loop)
{
    db::XSpriteItem *ptr = database.findTile(m_type);
    if (ptr != 0)
        m_xsprite.playAnim(ptr->findAnim(anim), loop);
}
