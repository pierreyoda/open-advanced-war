#ifndef ARMYGENERAL_HPP
#define ARMYGENERAL_HPP

#include <list>
#include <boost/serialization/serialization.hpp>

namespace sf
{
    class RenderTarget;
}
class ArmyGeneral;
class Unit;

void drawArmy(sf::RenderTarget &target, ArmyGeneral &army);

typedef std::list<Unit*> l_units;

/** \brief An army general, that handles units, which are controlled by a human player or an AI.
*/
class ArmyGeneral
{
    friend class boost::serialization::access;
    friend void drawArmy(sf::RenderTarget &target, ArmyGeneral &army); // for drawing (external)

    public:
        ArmyGeneral();
        ~ArmyGeneral();

    private:
        template <typename Archive>
        void serialize(Archive &ar, const unsigned int &version)
        {
            ar &BOOST_SERIALIZATION_NVP(m_faction);
            ar &BOOST_SERIALIZATION_NVP(m_units);
        }

        std::string m_faction; /**< Faction. */
        l_units m_units; /**< Units. */
};

#endif /* ARMYGENERAL_HPP */
