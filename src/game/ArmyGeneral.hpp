#ifndef ARMYGENERAL_HPP
#define ARMYGENERAL_HPP

#include "../db/Database.hpp"

/** \brief An army general, that handles units, which are controlled by a human player or an AI.
*/
class ArmyGeneral
{
    friend class boost::serialization::access;

    public:
        ArmyGeneral();
        ~ArmyGeneral();

    private:
        template <typename Archive>
        void serialize(Archive &ar, const unsigned int &version)
        {
            ar &BOOST_SERIALIZATION_NVP(m_faction);
        }

        std::string m_faction; /**< Faction. */
};

#endif /* ARMYGENERAL_HPP */
