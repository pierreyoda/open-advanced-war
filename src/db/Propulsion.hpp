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
        friend class boost::serialization::access;

        public:
            /**
            * \brief Default constructor.
            * \param name Propulsion name (example : "wheels" "feet").
            */
            Propulsion (const std::string &name) : DatabaseItem(name)
            { }

        private:
            Propulsion() : DatabaseItem("")
            { }

            template <class Archive>
            void serialize(Archive &ar, const unsigned int &version)
            {
                ar &boost::serialization::make_nvp("DatabaseItem",
                    boost::serialization::base_object<DatabaseItem>(*this));
            }
    };
} /* End of namespace db */

#endif /* PROPULSION_HPP */
