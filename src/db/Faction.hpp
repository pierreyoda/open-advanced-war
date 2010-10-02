#ifndef FACTION_HPP
#define FACTION_HPP

#include "DatabaseItem.hpp"

namespace db
{
    /**
    * \brief Contains and provides faction informations.
    */
    class Faction : public DatabaseItem
    {
        friend class boost::serialization::access;

        public:
            /**
            * \brief Default constructor.
            * \param name Faction name (example : "Orange Star", "URSS", "USA").
            */
            Faction(const std::string &name) : DatabaseItem(name)
            { }

            //Faction &addUniqueUnit(const Unit &unit) /// TODO

        private:
            Faction() : DatabaseItem("")
            { }

            template <class Archive>
            void serialize(Archive &ar, const unsigned int &version)
            {
                ar &boost::serialization::make_nvp("DatabaseItem",
                    boost::serialization::base_object<DatabaseItem>(*this));
            }
    };
} /* End of namespace db */

#endif /* FACTION_HPP */
