#include "GameEntity.hpp"
#include "../GameGlobals.hpp"
#include "../db/Database.hpp"

GameEntity::GameEntity(const std::string &type) :
    m_class(findClassFromType(type)), m_pos(0, 0), m_type(type),
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
    m_xsprite.SetPosition(tilesToPixels(m_pos));
}

void GameEntity::setOrientation(const Orientation &orientation)
{
    if (orientation != UNDEFINED) // For error only
        m_orientation = orientation;
}

void GameEntity::playAnim(const std::string &anim, const bool &loop)
{
    db::XSpriteItem *ptr = 0;
    if (m_class == TILE)
        ptr = database.findTile(m_type);
    else if (m_class == BUILDING)
        ptr = database.findBuilding(m_type);
    else if (m_class == UNIT)
        ptr = database.findUnit(m_type);
    if (ptr != 0)
        m_xsprite.playAnim(ptr->findAnim(anim), loop);
}

Classes GameEntity::findClassFromType(const std::string &type)
{
    if (database.findTile(type) != 0)
        return TILE;
    if (database.findBuilding(type) != 0)
        return BUILDING;
    if (database.findUnit(type) != 0)
        return UNIT;
    return NONE;
}

GameEntity &GameEntity::setIntCaracteristic(const std::string &name,
    const int &value)
{
    db::addCaracteristic<int>(name, value, m_intCaracteristics);
    return *this;
}
GameEntity &GameEntity::setBoolCaracteristic(const std::string &name,
    const bool &value)
{
    db::addCaracteristic<bool>(name, value, m_boolCaracteristics);
    return *this;
}
GameEntity &GameEntity::setStringCaracteristic(const std::string &name,
    const std::string &value)
{
    db::addCaracteristic<std::string>(name, value, m_stringCaracteristics);
    return *this;
}

int GameEntity::getIntCaracteristic(const std::string &name,
    const int &default_)
{
    db::IntCaracteristic *ptr = db::findCaracteristic<int>(name,
        m_intCaracteristics);
    if (ptr == 0) // not found
        return default_;
    return ptr->value;
}
std::string GameEntity::getStringCaracteristic(const std::string &name)
{
    db::StringCaracteristic *ptr = db::findCaracteristic<std::string>(name,
        m_stringCaracteristics);
    if (ptr == 0) // not found
        return std::string();
    return ptr->value;
}
Tribool GameEntity::getBoolCaracteristic(const std::string &name)
{
    db::BoolCaracteristic *ptr = db::findCaracteristic<bool>(name,
        m_boolCaracteristics);
    if (ptr == 0) // not found
        return INDETERMINATE; // indeterminated value
    return boolToTribool(ptr->value);
}

sf::Vector2i GameEntity::pixelsToTiles(const sf::Vector2i &pos)
{
    return pixelsToTiles(pos.x, pos.y);
}

sf::Vector2i GameEntity::pixelsToTiles(const sf::Vector2f &pos)
{
    return pixelsToTiles(pos.x, pos.y);
}

sf::Vector2i GameEntity::pixelsToTiles(const int &posX, const int &posY)
{
    return sf::Vector2i(posX / gg.case_w, posY / gg.case_h);
}

sf::Vector2f GameEntity::tilesToPixels(const sf::Vector2i &pos)
{
    return sf::Vector2f(pos.x * gg.case_w, pos.y * gg.case_h);
}
