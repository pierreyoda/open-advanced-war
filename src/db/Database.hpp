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
#include "../tools/Singleton.hpp"

/** \namespace db
* \brief Namespace containing game database.
*/
namespace db
{
    /**
    * \brief Handles the module database. Contains all game informations (example : units). Singleton.
     */
    class Database : public Singleton<Database>
    {
        friend class Singleton<Database>;
        template<class Archive> friend
        void serialize(Archive &ar, Database &db, const unsigned int &version);

        public:
            void setModuleName(const std::string &moduleName)
            {
                if (!moduleName.empty())
                    m_moduleName = moduleName;
            }
            const std::string &getModuleName() const { return m_moduleName; }

            TranslationProvider &translationsRef() { return m_translations; }

            /** \brief Add a tile to the database (if not present yet).
            *
            * \param tile Tile to add (pointer to avoid crashes).
            * \return Reference to self.
            */
            Database &addTile(const Tile *tile);

            Tile *findTile(const std::string &item);
            Building *findBuilding(const std::string &item);
            Weapon *findWeapon(const std::string &item);
            Propulsion *findPropulsion(const std::string &item);
            Unit *findUnit(const std::string &item);
            Faction *findFaction(const std::string &item);

            /** \brief Checks if item exists. Searchs in all lists (but translations).
            *
            * \param item Item's name.
            * \return True if existing, false otherwise.
            */
            bool itemExists(const std::string &item);

        private:
            /**
            * \brief Default constructor.
            * \param moduleName The module name (example : "Vanilla", "WWII").
            */
            Database();
            ~Database();

            template <typename Derived>
            Derived *findItemIn(const std::string &item, std::list<Derived> &in)
            {
                for (typename std::list<Derived>::iterator iter = in.begin();
                    iter != in.end(); iter++)
                if (iter->name() == item)
                    return &*iter;
                return 0;
            }

            std::string m_moduleName; /** < Module name. */
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
        ar &db.m_moduleName;
        ar &db.m_tiles, &db.m_buildings, &db.m_weapons, &db.m_propulsions,
            &db.m_units, &db.m_factions, &db.m_translations;
    }
} /* End of namespace db */

extern db::Database *database;

BOOST_CLASS_VERSION(db::Database, 1)

#endif /* DATABASE_HPP */
