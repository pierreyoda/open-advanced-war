#include "GameEntity.hpp"
#include "../GameGlobals.hpp"
#include "../db/Database.hpp"

GameEntity::GameEntity(const std::string &name) : m_name(name), m_alias(/*TODO*/),
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

void GameEntity::playAnim(const std::string &anim, const bool &loop)
{
    db::XSpriteItem *ptr = database->findTile(m_name);
    if (ptr != 0)
        XSprite::playAnim(ptr->findAnim(anim), loop);
}
