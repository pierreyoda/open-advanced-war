#include <SFML/Graphics.hpp>
#include "ArmyGeneral.hpp"
#include "../db/Database.hpp"
#include "Unit.hpp"

using namespace std;

ArmyGeneral::ArmyGeneral(const unsigned int &id, const string &faction) :
    m_id(id), m_faction(faction)
{

}

ArmyGeneral::~ArmyGeneral()
{
    for (std::list<Unit*>::iterator iter = m_units.begin();
        iter != m_units.end(); iter++)
        delete *iter;
    m_units.clear();
}

void ArmyGeneral::addUnit(const sf::Vector2i &position, const string &type)
{
    if (database.findUnit(type) == 0) // not in database
        return;
    if (position.x < 0 || position.y < 0) // invalid position
        return;
    Unit *unit = new Unit(type, m_units.size());
        unit->setPosition(position);
    m_units.push_back(unit);
}

unsigned int ArmyGeneral::getUnitId(sf::Vector2i &pos)
{
    for (l_units::iterator iter = m_units.begin(); iter != m_units.end(); iter++)
        if ((*iter) != 0 && (*iter)->position() == pos)
            return (*iter)->id();
    return 0; // not found
}

void drawArmy(sf::RenderTarget &target, ArmyGeneral &army)
{
    l_units &units = army.m_units;
    for (l_units::iterator iter = units.begin(); iter != units.end(); iter++)
    {
        Unit *unit = (*iter);
        if (unit == 0)
            continue;
        unit->xsprite().update();
        target.Draw(unit->xspriteConst());
    }
}
