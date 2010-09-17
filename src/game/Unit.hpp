#ifndef UNIT_GAMEENTITY_HPP
#define UNIT_GAMEENTITY_HPP

#include "GameEntity.hpp"

/** \brief A Unit is an extended GameEntity.
*/
class Unit : public GameEntity
{
    friend class boost::serialization::access;

    public:
        Unit(const std::string &type);
        virtual ~Unit();

    private:
        template <typename Archive>
        void serialize(Archive &ar, const unsigned int &version)
        {
            ar &boost::serialization::make_nvp("GameEntity",
                boost::serialization::base_object<GameEntity>(*this));
        }
};

#endif /* UNIT_GAMEENTITY_HPP */
