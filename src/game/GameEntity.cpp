#include "GameEntity.hpp"
#include "../GameGlobals.hpp"
#include "../db/Database.hpp"

GameEntity::GameEntity(const std::string &type) :
    m_classe(findClasseFromType(type)), m_pos(0, 0), m_type(type),
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

void GameEntity::setOrientation(const Orientation &orientation)
{
    if (orientation != UNDEFINED) // For error only
        m_orientation = orientation;
}

void GameEntity::playAnim(const std::string &anim, const bool &loop)
{
    db::XSpriteItem *ptr = database.findTile(m_type);
    if (ptr == 0)
        ptr = database.findBuilding(m_type);
    if (ptr == 0)
        ptr = database.findUnit(m_type); // Search successively in all XSpriteItem (or derived) list
    if (ptr != 0)
        m_xsprite.playAnim(ptr->findAnim(anim), loop);
}

Classes GameEntity::findClasseFromType(const std::string &type)
{
    if (database.findTile(type) != 0)
        return TILE;
    if (database.findBuilding(type) != 0)
        return BUILDING;
    if (database.findUnit(type) != 0)
        return UNIT;
    return NONE;
}
