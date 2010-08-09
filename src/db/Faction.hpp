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
        public:
            /**
            * \brief Default constructor.
            * \param name Faction name (example : "Orange Star", "Red").
            */
            Faction(const std::string &name) : DatabaseItem(name)
            { }
    };

    template<class Archive>
    void serialize(Archive &ar, Faction &faction, const unsigned int &version)
    {
        ar &boost::serialization::base_object<DatabaseItem>(faction);
    }
} /* End of namespace db */

#endif /* FACTION_HPP */
