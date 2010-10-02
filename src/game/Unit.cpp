#include "Unit.hpp"

Unit::Unit(const std::string &type, const std::string &faction,
   const unsigned int &id) : GameEntity(type, faction), m_id(id)
{

}

Unit::~Unit()
{

}
