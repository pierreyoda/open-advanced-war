#ifndef PROPULSION_HPP
#define PROPULSION_HPP

#include "DatabaseItem.hpp"

namespace db
{
    typedef std::pair<std::string, unsigned int> p_canMoveTo;

    /**
    * \brief Corresponds to a propulsion system (here propulsion is how the unit moves).
    */
    class Propulsion : public DatabaseItem
    {
        friend class boost::serialization::access;

        public:
            /** \brief Default constructor.
            *
            * \param name Propulsion name (example : "wheels" "feet").
            */
            Propulsion (const std::string &name) : DatabaseItem(name)
            { }

            /** \brief Add a possible tile where the propulsion can make a unit go.
            *
            * \brief tile Tile's name.
            * \brief cost Tile's cost required to move.
            * \return Reference to self.
            */
            Propulsion &addCanMoveTo(const std::string &tile,
                const unsigned int &cost)
            {
                if (tile.empty())
                    return *this;
                p_canMoveTo pair(tile, cost);
                if (findCanMoveTo(tile) == 0) // Does no already exist
                    m_canMoveTo.push_back(pair);
                return *this;
            }

            bool canMoveTo(const std::string &tile) const
            {
                if (tile.empty())
                    return false;
                return (findCanMoveToConst(tile) != 0); // in list or not
            }

            unsigned int moveCost(const std::string &tile) const
            {
                if (tile.empty())
                    return 0;
                const p_canMoveTo *ptr = findCanMoveToConst(tile);
                if (ptr == 0)
                    return 0;
                return ptr->second;
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

            p_canMoveTo *findCanMoveTo(const std::string &tile)
            {
                for (std::list<p_canMoveTo>::iterator iter = m_canMoveTo.begin();
                    iter != m_canMoveTo.end(); iter++)
                    if (iter->first == tile)
                        return &*iter;
                return 0;
            }
            const p_canMoveTo *findCanMoveToConst(const std::string &tile) const
            {
                for (std::list<p_canMoveTo>::const_iterator iter =
                     m_canMoveTo.begin(); iter != m_canMoveTo.end(); iter++)
                    if (iter->first == tile)
                        return &*iter;
                return 0;
            }

            std::list<p_canMoveTo> m_canMoveTo;
    };
} /* End of namespace db */

#endif /* PROPULSION_HPP */
