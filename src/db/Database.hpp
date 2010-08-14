#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <list>
#include "Tile.hpp"
#include "Building.hpp"
#include "Weapon.hpp"
#include "Propulsion.hpp"
#include "Unit.hpp"
#include "Faction.hpp"
#include "TranslationProvider.hpp"

namespace db
{
    /**
    * \brief Handles the module database. Contains all game informations (example : units).
     */
    class Database : public DatabaseItem
    {
        public:
            /**
            * \brief Default constructor.
            * \param moduleName The module name (example : "Vanilla", "WWII").
            */
            Database(const std::string &moduleName);
            ~Database();

            TranslationProvider &translationsRef() { return m_translations; }

        private:
            std::list<Tile> m_tiles; /**<  List of tiles. */
            std::list<Building> m_buildings; /**<  List of buildings. */
            std::list<Weapon> m_weapons; /**<  List of weapon systems. */
            std::list<Propulsion> m_propulsions; /**<  List of propulsions. */
            std::list<Unit> m_units; /**<  List of "shared" units. */
            std::list<Faction> m_factions; /**<  List of factions. */
            TranslationProvider m_translations; /** < Provides translations. **/
    };

    template<class Archive>
    void serialize(Archive &ar, Database &db, const unsigned int &version)
    {
        ar &boost::serialization::base_object<DatabaseItem>(db);
        ar &db.m_tiles, &db.m_buildings, &db.m_weapons, &db.m_propulsions,
            &db.m_units, &db.m_factions, &db.m_translations;
    }

} /* End of namespace db */

/*void importFromXml(const std::string &file, db::Database &db)
{

}
void exportToXml(const std::string &file, const db::Database &db)
{

}*/

BOOST_CLASS_VERSION(db::Database, 1)

#endif /* DATABASE_HPP */
