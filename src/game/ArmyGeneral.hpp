#ifndef ARMYGENERAL_HPP
#define ARMYGENERAL_HPP

#include <list>
#include <boost/serialization/serialization.hpp>
#include <SFML/System/Vector2.hpp>
#include "Unit.hpp"

namespace sf
{
    class RenderTarget;
}
class ArmyGeneral;

void drawArmy(sf::RenderTarget &target, ArmyGeneral &army);

typedef std::list<Unit*> l_units;

/** \brief An army general, that handles units, which are controlled by a human player or an AI.
*/
class ArmyGeneral
{
    friend class boost::serialization::access;
    friend void drawArmy(sf::RenderTarget &target, ArmyGeneral &army); // for drawing (external)

    public:
        ArmyGeneral(const unsigned int &id, const std::string &faction);
        ~ArmyGeneral();

        void addUnit(const sf::Vector2i &position, const std::string &type);

        unsigned int getUnitId(const sf::Vector2i &pos);
        Unit *getUnitPtr(const sf::Vector2i &pos);

        unsigned int id() const { return m_id; }

    private:
        template <typename Archive>
        void serialize(Archive &ar, const unsigned int &version)
        {
            ar &BOOST_SERIALIZATION_NVP(m_id);
            ar &BOOST_SERIALIZATION_NVP(m_faction);
            ar &BOOST_SERIALIZATION_NVP(m_units);
        }

        unsigned int m_id; /**< Unique identifier. */
        std::string m_faction; /**< Faction. */
        l_units m_units; /**< Units. */
};

#endif /* ARMYGENERAL_HPP */
