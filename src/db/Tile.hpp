#ifndef TILE_HPP
#define TILE_HPP

#include "XSpriteItem.hpp"

namespace db
{
    /**
    * \brief Contains informations to create a tile.
    */
    class Tile : public XSpriteItem
    {
        public:
            /** \brief Default constructor.
            *
            * \param name The tile name (example : "river").
            */
            Tile(const std::string &name) : XSpriteItem(name), m_rotating(false),
                m_protection(0)
            { }
            /** \brief Constructor.
            *
            * \param name The tile name (example : "river").
            * \param rotating Orientable.
            */
            Tile(const std::string &name, const bool &rotating) :
                XSpriteItem(name), m_rotating(rotating), m_protection(0)
            { }

            void setProtection(const unsigned int &protection)
            {
                m_protection = protection;
            }
            const bool &isOrientable() { return m_rotating; }

        private:
            const bool m_rotating; /**<  Is orientable. */
            unsigned m_protection; /** < Protection level. */
    };

    template<class Archive>
    void serialize(Archive &ar, Tile &tile, const unsigned int &version)
    {
        ar &boost::serialization::base_object<XSpriteItem>(tile);
        ar &tile.m_rotating &tile.m_protection;
    }
} /* End of namespace db */

#endif /* TILE_HPP */
