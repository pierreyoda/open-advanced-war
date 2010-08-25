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
        friend class boost::serialization::access;

        public:
            /**
            * \brief Default constructor.
            * \param name Weapon name (example : "bazooka", "75canon").
            */
            Weapon(const std::string &name) : DatabaseItem(name)
            { }

        private:
            template <class Archive>
            void serialize(Archive &ar, const unsigned int &version)
            {
                ar &boost::serialization::make_nvp("DatabaseItem",
                    boost::serialization::base_object<DatabaseItem>(*this));
            }
    };
} /* End of namespace db */

#endif /* WEAPON_HPP */
