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
#include <boost/serialization/utility.hpp>

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
        friend class boost::serialization::access;

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

            // Lua side (const pointer) -- Crashes when trying to modify!
            /*const Tile *findTile(const std::string &item) const;
            const Building *findBuilding(const std::string &item) const;
            const Weapon *findWeapon(const std::string &item) const;
            const Propulsion *findPropulsion(const std::string &item) const;
            const Unit *findUnit(const std::string &item) const;
            const Faction *findFaction(const std::string &item) const;*/

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

            template<class Archive>
            void serialize(Archive &ar, const unsigned int &version)
            {
                ar &BOOST_SERIALIZATION_NVP(m_moduleName);
                ar &BOOST_SERIALIZATION_NVP(m_tiles);
                ar &BOOST_SERIALIZATION_NVP(m_buildings);
                ar &BOOST_SERIALIZATION_NVP(m_weapons);
                ar &BOOST_SERIALIZATION_NVP(m_propulsions);
                ar &BOOST_SERIALIZATION_NVP(m_units);
                ar &BOOST_SERIALIZATION_NVP(m_factions);
                ar &BOOST_SERIALIZATION_NVP(m_translations);
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
} /* End of namespace db */

extern db::Database &database;

/* Centralization of serialization versionning. */
BOOST_CLASS_VERSION(db::Database, 1)
BOOST_CLASS_VERSION(db::DatabaseItem, 1)
BOOST_CLASS_VERSION(db::Frame, 1);
BOOST_CLASS_VERSION(db::Animation, 1);
BOOST_CLASS_VERSION(db::XSpriteItem, 1);

#endif /* DATABASE_HPP */
