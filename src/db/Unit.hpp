#ifndef UNIT_HPP
#define UNIT_HPP

#include "XSpriteItem.hpp"

namespace db
{
    /**
    * \brief Contains informations to create a tile.
    */
    class Unit : public XSpriteItem
    {
        public:
            /**
            * \brief Default constructor.
            * \param name The tile name (example : "light tank", "infantry").
            */
            Unit(const std::string &name) : XSpriteItem(name)
            {

            }
    };

    template<class Archive>
    void serialize(Archive &ar, Unit &unit, const unsigned int &version)
    {
        ar &boost::serialization::base_object<XSpriteItem>(unit);
    }
} /* End of namespace db */

#endif /* UNIT_HPP */
