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
        public:
            /**
            * \brief Default constructor.
            * \param name The building name (example : "City" or "Tank factory").
            */
            Building(const std::string &name, const int &gainPerTurn) :
                XSpriteItem(name), m_gainPerTurn(gainPerTurn)
            { }

        private:
            const int m_gainPerTurn; /**<  Gain per turn (example : 1000). Can be negative. */
    };

    template<class Archive>
    void serialize(Archive &ar, Building &building, const unsigned int &version)
    {
        ar &boost::serialization::base_object<XSpriteItem>(building);
        ar &building.m_gainPerTurn;
    }
} /* End of namespace db */

#endif /* BUILDING_HPP */
