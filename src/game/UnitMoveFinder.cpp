#include <algorithm>
#include "UnitMoveFinder.hpp"
#include "../db/Database.hpp"
#include "../lua/LuaVirtualMachine.hpp"
#include "../Map.hpp"

using namespace std;

bool UnitMoveFinder::run(const Unit &unit, const Map &map, l_Vector2i &canMoveTo)
{
    static bool luaError = false;
    if (luaError)
        return false;
    const sf::Vector2i startPos = unit.position();
    if (!map.isInsideMap(startPos))
        return false;
    l_Vector2i alreadyAdded;
    unsigned int move = 0;
    /*if (!addAdjacentTiles(map, unit, startPos, alreadyAdded,))
        return false;*/
    CALL_LUA_RFUNCTION(LuaVM::getInstance().getLua(), unsigned int, move,
        "getUnitMovePoints", luaError, &unit)
    if (luaError)
        return false;
    return true;
}

bool UnitMoveFinder::addAdjacentTiles(const Map &map, const Unit &unit,
    sf::Vector2i &pos, l_Vector2i &alreadyAdded,
    const unsigned int &remainingCurrency)
{
    list<const GameEntity*> cases;
    static const GameEntity *nullPtr = 0;
    // Getting all cases
    cases.push_back(map.getTileConstPtr(pos.x-1, pos.y)); // left
    cases.push_back(map.getTileConstPtr(pos.x+1, pos.y)); // right
    cases.push_back(map.getTileConstPtr(pos.x, pos.y-1)); // up
    cases.push_back(map.getTileConstPtr(pos.x, pos.y+1)); // down
    // Clearing invalid pointers
    cases.erase(remove(cases.begin(), cases.end(), nullPtr), cases.end());

    if (cases.empty()) // if no valid adjacent cases...
        return false; // nothing has been added
    bool somethingAdded = false;
    // Adding if needed
    for (list<const GameEntity*>::iterator iter = cases.begin();
        iter != cases.end(); iter++)
    {
        /*db::Propulsion *ptr = database.findPropulsion(unit.getStringCaracteristic("propulsion"));
        if (ptr == 0 || !ptr->canMoveTo((*iter)->type())) // propulsion not found in database / unit cannot move to that tile
            continue;
        if (ptr->moveCost((*iter)->type()) <= remainingCurrency && // if unit can "pay" movement cost...
            !tileAlreadyAdded(alreadyAdded, (*iter)->position())) // and tile not already added
        {
            alreadyAdded.push_back((*iter)->position()); // adding it
            somethingAdded = true;
        }*/
    }
    return somethingAdded;
}

bool UnitMoveFinder::tileAlreadyAdded(const l_Vector2i &in,
    const sf::Vector2i &tilePos)
{
    for (l_Vector2i::const_iterator iter = in.begin(); iter  != in.end();
        iter ++)
    {
        if (*iter == tilePos)
            return true; // tilePos is present and so already added
    }
    return false; // tilePos is not present  and so not already added
}

bool UnitMoveFinder::canUnitMoveTo(const string &unit, const string &tile)
{
    const db::Unit *ptr = database.findUnit(unit);
    if (ptr == 0)
        return false;
    const db::Propulsion *ptr2 = database.findPropulsion(ptr->propulsion());
    if (ptr2 == 0)
        return false;
    return (ptr2->canMoveTo(tile));
}
