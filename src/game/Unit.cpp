#include "Unit.hpp"

Unit::Unit(const std::string &type, const unsigned int &id) : GameEntity(type),
    m_id(id)
{

}

Unit::~Unit()
{

}
