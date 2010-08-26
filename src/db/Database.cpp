#include <iostream>
#include "Database.hpp"

using namespace std;

db::Database *database = &db::Database::getInstance();

namespace db
{
    Database::Database() : m_moduleName("unknown")
    {

    }

    Database::~Database()
    {

    }

    Database &Database::addTile(const Tile *tile)
    {
        if (tile == 0)
            return *this;
        if (itemExists(tile->name()))
        {
            std::cerr << "Warning : item name '" << tile->name()
                << "' already exists in database.\n";
            return *this;
        }
        m_tiles.push_back(*tile);
        return *this;
    }

    bool Database::itemExists(const std::string &item)
    {
        return (findTile(item) || findBuilding(item) || findWeapon(item)
            || findPropulsion(item) || findUnit(item) || findFaction(item));
    }

    Tile *Database::findTile(const std::string &item)
    {
        return findItemIn<Tile>(item, m_tiles);
    }
    Building *Database::findBuilding(const std::string &item)
    {
        return findItemIn<Building>(item, m_buildings);
    }
    Weapon *Database::findWeapon(const std::string &item)
    {
        return findItemIn<Weapon>(item, m_weapons);
    }
    Propulsion *Database::findPropulsion(const std::string &item)
    {
        return findItemIn<Propulsion>(item, m_propulsions);
    }
    Unit *Database::findUnit(const std::string &item)
    {
        return findItemIn<Unit>(item, m_units);
    }
    Faction *Database::findFaction(const std::string &item)
    {
        return findItemIn<Faction>(item, m_factions);
    }

    /*const Tile *Database::findTile(const std::string &item) const
    {
        return findItemInConst<Tile>(item, m_tiles);
    }
    const Building *Database::findBuilding(const std::string &item) const
    {
        return findItemInConst<Building>(item, m_buildings);
    }
    const Weapon *Database::findWeapon(const std::string &item) const
    {
        return findItemInConst<Weapon>(item, m_weapons);
    }
    const Propulsion *Database::findPropulsion(const std::string &item) const
    {
        return findItemInConst<Propulsion>(item, m_propulsions);
    }
    const Unit *Database::findUnit(const std::string &item) const
    {
        return findItemInConst<Unit>(item, m_units);
    }
    const Faction *Database::findFaction(const std::string &item) const
    {
        return findItemInConst<Faction>(item, m_factions);
    }*/
} /* End of namespace db */
