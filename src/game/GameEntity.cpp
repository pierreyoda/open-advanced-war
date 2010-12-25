#include "GameEntity.hpp"
#include "../GameGlobals.hpp"
#include "../db/Database.hpp"

GameEntity::GameEntity(const std::string &type, const std::string &faction) :
    m_class(findClassFromType(type)), m_pos(0, 0), m_type(type),
    m_alias(/*TODO*/), m_faction(faction), m_ownerId(0), m_orientation(RIGHT)
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
    const db::XSpriteItem *ptr = 0;
    if (m_class == TILE)
        ptr = database.findTile(m_type);
    else if (m_class == BUILDING)
        ptr = database.findBuilding(m_type);
    else if (m_class == UNIT)
        ptr = database.findUnit(m_type);
    if (ptr == 0)
        return;
    m_xsprite.playAnim(ptr->findAnim(anim, m_faction), loop);
    sf::IntRect subRect(m_xsprite.GetSubRect());
    if (subRect.Height > (int)gg.case_h)
        m_xsprite.SetOrigin(0, subRect.Height-gg.case_h);
}

Classes GameEntity::findClassFromType(const std::string &type)
{
    if (type.empty())
            return NONE;
    if (database.findTile(type) != 0)
        return TILE;
    if (database.findBuilding(type) != 0)
        return BUILDING;
    if (database.findUnit(type) != 0)
        return UNIT;
    return NONE;
}

GameEntity &GameEntity::setIntFeature(const std::string &name,
    const int &value)
{
    db::addFeature<int>(name, value, m_intFeatures);
    return *this;
}
GameEntity &GameEntity::setStringFeature(const std::string &name,
    const std::string &value)
{
    db::addFeature<std::string>(name, value, m_stringFeatures);
    return *this;
}

int GameEntity::getIntFeature(const std::string &name,
    const int &default_)
{
    db::IntFeature *ptr = db::findFeature<int>(name,
        m_intFeatures);
    if (ptr == 0) // not found
        return default_;
    return ptr->value;
}
std::string GameEntity::getStringFeature(const std::string &name)
{
    db::StringFeature *ptr = db::findFeature<std::string>(name,
        m_stringFeatures);
    if (ptr == 0) // not found
        return std::string();
    return ptr->value;
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
