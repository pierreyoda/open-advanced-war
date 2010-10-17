#include <SFML/Graphics.hpp>
#include "ArmyGeneral.hpp"
#include "../db/Database.hpp"
#include "../lua/LuaVirtualMachine.hpp"
#include "../Game.hpp"
#include "Unit.hpp"

using namespace std;

ArmyGeneral::ArmyGeneral(const unsigned int &id, const string &faction) :
    m_id(id), m_faction(faction)
{

}

ArmyGeneral::~ArmyGeneral()
{
    for (list<Unit*>::iterator iter = m_units.begin();
        iter != m_units.end(); iter++)
        delete *iter;
    m_units.clear();
}

bool ArmyGeneral::addUnit(const sf::Vector2i &pos, const string &type,
    const string &faction)
{
    string usedFaction = faction;
    if (faction.empty())
        usedFaction = m_faction;
    if (pos.x < 0 || pos.y < 0) // invalid position
        return false;
    if (getUnitPtr(pos) != 0) // other unit already present
        return false;
    const db::Unit *unitDb = database.findUnit(type);
    if (unitDb == 0) // not in database
        return false;
    static bool luaError = false, luaError2 = false;
    bool canSpawnUnit = false;
    if (!luaError)
        CALL_LUA_RFUNCTION(LuaVM::getInstance().getLua(), bool, canSpawnUnit,
            "canPlaceUnit", luaError, unitDb, pos)
    if (luaError || !canSpawnUnit)
        return false;
    Unit *unit = new Unit(type, usedFaction, m_units.size());
        unit->setPosition(pos);
        unit->playAnim("base");
    if (!luaError2)
        CALL_LUA_FUNCTION(LuaVM::getInstance().getLua(), void,
            "onGameEntityPlaced", luaError2, unit)
    m_units.push_back(unit);
    return true;
}

bool ArmyGeneral::removeUnit(const sf::Vector2i &pos)
{
    l_units::iterator iter = getUnitIter(pos);
    if (iter == m_units.end())
        return false;
    delete *iter;
    *iter = 0;
    m_units.erase(iter);
    gGame.unitDeleted(pos);
    return true;
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
    l_units::iterator iter = getUnitIter(pos);
    if (iter == m_units.end())
        return 0;
    return *iter;
}

l_units::iterator ArmyGeneral::getUnitIter(const sf::Vector2i &pos)
{
    for (l_units::iterator iter = m_units.begin(); iter != m_units.end(); iter++)
        if ((*iter)->position() == pos)
                return iter;
    return m_units.end(); // not found
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
