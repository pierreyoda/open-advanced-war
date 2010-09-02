#include <iostream>
#include "Database.hpp"

using namespace std;

db::Database &database = db::Database::getInstance();

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
        if (tile != 0)
            addItem<Tile>(*tile, m_tiles);
        return *this;
    }
    Database &Database::addUnit(const Unit *unit)
    {
        if (unit != 0)
            addItem<Unit>(*unit, m_units);
        return *this;
    }
    Database &Database::addBuilding(const Building *building)
    {
        if (building != 0)
            addItem<Building>(*building, m_buildings);
        return *this;
    }
    Database &Database::addCategory(const Category *category)
    {
        if (category != 0)
            addItem<Category>(*category, m_categories);
        return *this;
    }

    bool Database::itemExists(const std::string &item)
    {
        return (findTile(item) || findBuilding(item) || findWeapon(item)
            || findPropulsion(item) || findUnit(item) || findFaction(item)
            || findCategory(item));
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
    Category *Database::findCategory(const std::string &item)
    {
        return findItemIn<Category>(item, m_categories);
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
