#include <SFML/Graphics.hpp>
#include "ArmyGeneral.hpp"
#include "../db/Database.hpp"
#include "../lua/LuaVirtualMachine.hpp"
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

void ArmyGeneral::addUnit(const sf::Vector2i &position, const string &type,
    const std::string &faction)
{
    std::string usedFaction = faction;
    if (faction.empty())
        usedFaction = m_faction;
    if (position.x < 0 || position.y < 0) // invalid position
        return;
    if (getUnitPtr(position) != 0) // other unit already present
        return;
    const db::Unit *unitDb = database.findUnit(type);
    if (unitDb == 0) // not in database
        return;
    static bool luaError = false, luaError2 = false;
    bool canSpawnUnit = false;
    if (!luaError)
        CALL_LUA_RFUNCTION(LuaVM::getInstance().getLua(), bool, canSpawnUnit,
            "canPlaceUnit", luaError, unitDb, position)
    if (!canSpawnUnit)
        return;
    Unit *unit = new Unit(type, usedFaction, m_units.size());
        unit->setPosition(position);
        unit->playAnim("base");
    if (!luaError2)
        CALL_LUA_FUNCTION(LuaVM::getInstance().getLua(), void,
            "onGameEntityPlaced", luaError2, unit)
    m_units.push_back(unit);
}

unsigned int ArmyGeneral::getUnitId(const sf::Vector2i &pos)
{
    Unit *ptr = getUnitPtr(pos);
    if (ptr != 0)
        return ptr->id();
    return 0; // not found
}

Unit *ArmyGeneral::getUnitPtr(const sf::Vector2i &pos)
{
    for (l_units::iterator iter = m_units.begin(); iter != m_units.end(); iter++)
        if ((*iter)->position() == pos)
                return (*iter);
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
