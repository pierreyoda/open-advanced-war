#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "DatabaseItem.hpp"

namespace db
{
    /**
    * \brief Corresponds to a weapon system.
    */
    class Weapon : public DatabaseItem
    {
        public:
            /**
            * \brief Default constructor.
            * \param name Weapon name (example : "bazooka", "75canon").
            */
            Weapon(const std::string &name) : DatabaseItem(name)
            { }
    };

    template<class Archive>
    void serialize(Archive &ar, Weapon &weapon, const unsigned int &version)
    {
        ar &boost::serialization::base_object<DatabaseItem>(weapon);
    }
} /* End of namespace db */

#endif /* WEAPON_HPP */
