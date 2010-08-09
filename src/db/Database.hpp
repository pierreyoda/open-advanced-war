#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <list>
#include "Tile.hpp"
#include "Building.hpp"
#include "Weapon.hpp"
#include "Propulsion.hpp"
#include "Unit.hpp"
#include "Faction.hpp"

namespace db
{
    /**
    * \brief Contains informations to create an animated sprite.
    */
    struct AnimInfos
    {

    };

    /**
    * \brief Handles the module database. Contains all game informations (example : units).
     */
    class Database : public DatabaseItem
    {
        public:
            /**
            * \brief Default constructor.
            * \param moduleName The module name (example : "moduleWWII").
            */
            Database(const std::string &moduleName = "base");
            ~Database();

        private:
            std::list<Tile> m_tiles; /**<  List of tiles. */
            std::list<Building> m_buildings; /**<  List of buildings. */
            std::list<Weapon> m_weapons; /**<  List of weapon systems. */
            std::list<Propulsion> m_propulsions; /**<  List of propulsions. */
            std::list<Unit> m_units; /**<  List of "shared" units. */
            std::list<Faction> m_factions; /**<  List of factions. */
    };

    template<class Archive>
    void serialize(Archive &ar, Database &db, const unsigned int &version)
    {
        ar &boost::serialization::base_object<DatabaseItem>(db);
        ar &db.m_tiles, &db.m_buildings, &db.m_weapons, &db.m_propulsions,
            &db.m_units, &db.m_factions;
    }

} /* End of namespace db */

BOOST_CLASS_VERSION(db::Database, 1)

#endif /* DATABASE_HPP */
