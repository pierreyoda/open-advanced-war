#ifndef UNIT_HPP
#define UNIT_HPP

#include "XSpriteItem.hpp"

namespace db
{
    /** \brief A (template) class that represents a caracteristic (name and [maximum in most case] value).
    */
    template <typename Type>
    struct Caracteristic
    {
        std::string name;
        Type value;
    };

    typedef Caracteristic<int> IntCaracteristic;
    typedef Caracteristic<bool> BoolCaracteristic;

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
            Unit() : XSpriteItem("")
            { }

            template <class Archive>
            void serialize(Archive &ar, const unsigned int &version)
            {
                ar &boost::serialization::make_nvp("XSpriteItem",
                    boost::serialization::base_object<XSpriteItem>(*this));
            }
    };
} /* End of namespace db */

#endif /* UNIT_HPP */
