#ifndef UNIT_GAMEENTITY_HPP
#define UNIT_GAMEENTITY_HPP

#include "GameEntity.hpp"

/** \brief A Unit is an extended GameEntity.
*/
class Unit : public GameEntity
{
    friend class boost::serialization::access;

    public:
        Unit(const std::string &type, const unsigned int &id);
        virtual ~Unit();

        unsigned int id() const { return m_id; }

    private:
        template <typename Archive>
        void serialize(Archive &ar, const unsigned int &version)
        {
            ar &boost::serialization::make_nvp("GameEntity",
                boost::serialization::base_object<GameEntity>(*this));
            ar &BOOST_SERIALIZATION_NVP(m_id);
        }

        unsigned int m_id; /**< Unit's unique identifier. */
};

#endif /* UNIT_GAMEENTITY_HPP */
