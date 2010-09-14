#include <SFML/Graphics.hpp>
#include "ArmyGeneral.hpp"
#include "../db/Database.hpp"
#include "Unit.hpp"

ArmyGeneral::ArmyGeneral()
{

}

ArmyGeneral::~ArmyGeneral()
{

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
