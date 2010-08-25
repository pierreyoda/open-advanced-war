#ifndef BUILDING_HPP
#define BUILDING_HPP

#include "DatabaseItem.hpp"

namespace db
{
    /**
    * \brief Contains informations to create a building.
    */
    class Building : public XSpriteItem
    {
        friend class boost::serialization::access;

        public:
            /**
            * \brief Default constructor.
            * \param name The building name (example : "City" or "Tank factory").
            */
            Building(const std::string &name, const int &gainPerTurn) :
                XSpriteItem(name), m_gainPerTurn(gainPerTurn)
            { }

        private:
            template<class Archive>
            void serialize(Archive &ar, const unsigned int &version)
            {
                ar &boost::serialization::make_nvp("XSpriteItem",
                    boost::serialization::base_object<XSpriteItem>(*this));
                ar &BOOST_SERIALIZATION_NVP(m_gainPerTurn);
            }

            const int m_gainPerTurn; /**<  Gain per turn (example : 1000). Can be negative. */
    };
} /* End of namespace db */

#endif /* BUILDING_HPP */
