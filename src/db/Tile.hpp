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
        friend class boost::serialization::access;

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
            const unsigned int &protection() { return m_protection; }

        private:
            Tile() : XSpriteItem(""), m_rotating(false), m_protection(0)
            { }

            template<class Archive>
            void serialize(Archive &ar, const unsigned int &version)
            {
                ar &boost::serialization::make_nvp("XSpriteItem",
                    boost::serialization::base_object<XSpriteItem>(*this));
                ar &BOOST_SERIALIZATION_NVP(m_rotating);
                ar &BOOST_SERIALIZATION_NVP(m_protection);
            }

            bool m_rotating; /**<  Is orientable. */
            unsigned m_protection; /** < Protection level. */
    };
} /* End of namespace db */

#endif /* TILE_HPP */
