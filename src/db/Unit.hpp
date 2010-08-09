#ifndef UNIT_HPP
#define UNIT_HPP

#include "DatabaseItem.hpp"

namespace db
{
    /**
    * \brief Contains informations to create a tile.
    */
    class Unit : public DatabaseItem
    {
        public:
            /**
            * \brief Default constructor.
            * \param name The tile name (example : "light tank", "infantry").
            */
            Unit(const std::string &name) : DatabaseItem(name)
            {

            }
    };

    template<class Archive>
    void serialize(Archive &ar, Unit &unit, const unsigned int &version)
    {
        ar &boost::serialization::base_object<DatabaseItem>(unit);
    }
} /* End of namespace db */

#endif /* UNIT_HPP */
