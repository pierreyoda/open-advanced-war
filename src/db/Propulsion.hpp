#ifndef PROPULSION_HPP
#define PROPULSION_HPP

#include "DatabaseItem.hpp"

namespace db
{
    /**
    * \brief Corresponds to a propulsion system (here propulsion is how the unit moves).
    */
    class Propulsion : public DatabaseItem
    {
        public:
            /**
            * \brief Default constructor.
            * \param name Propulsion name (example : "wheels" "feet").
            */
            Propulsion (const std::string &name) : DatabaseItem(name)
            { }
    };

    template<class Archive>
    void serialize(Archive &ar, Propulsion &prop, const unsigned int &version)
    {
        ar &boost::serialization::base_object<DatabaseItem>(prop);
    }
} /* End of namespace db */

#endif /* PROPULSION_HPP */
