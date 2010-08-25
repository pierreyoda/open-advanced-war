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
        friend class boost::serialization::access;

        public:
            /**
            * \brief Default constructor.
            * \param name The tile name (example : "light tank", "infantry").
            */
            Unit(const std::string &name) : XSpriteItem(name)
            {

            }

        private:
            template <class Archive>
            void serialize(Archive &ar, const unsigned int &version)
            {
                ar &boost::serialization::make_nvp("XSpriteItem",
                    boost::serialization::base_object<XSpriteItem>(*this));
            }
    };
} /* End of namespace db */

#endif /* UNIT_HPP */
