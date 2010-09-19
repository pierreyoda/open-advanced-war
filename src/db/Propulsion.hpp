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

            Propulsion &addCanMoveTo(const std::string &tile)
            {
                if (tile.empty())
                    return *this;
                if (std::find(m_canMoveTo.begin(), m_canMoveTo.end(), tile) ==
                    m_canMoveTo.end()) // Does no already exist
                    m_canMoveTo.push_back(tile);
                return *this;
            }

            bool canMoveTo(const std::string &tile) const
            {
                if (tile.empty())
                    return false;
                return (std::find(m_canMoveTo.begin(), m_canMoveTo.end(), tile) !=
                    m_canMoveTo.end()); // in list or not
            }

        private:
            Propulsion() : DatabaseItem("")
            { }

            template <class Archive>
            void serialize(Archive &ar, const unsigned int &version)
            {
                ar &boost::serialization::make_nvp("DatabaseItem",
                    boost::serialization::base_object<DatabaseItem>(*this));
                ar &BOOST_SERIALIZATION_NVP(m_canMoveTo);
            }

            l_string m_canMoveTo;
    };
} /* End of namespace db */

#endif /* PROPULSION_HPP */
