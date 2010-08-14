#ifndef TILE_HPP
#define TILE_HPP

#include "XSpriteItem.hpp"

namespace db
{
    /**
    * \brief Contains informations to create a tile.
    */
    class Tile : public DatabaseItem
    {
        public:
            /**
            * \brief Default constructor.
            * \param name The tile name (example : "river").
            */
            Tile(const std::string &name, const bool &rotating = false) :
                DatabaseItem(name), m_rotating(rotating)
            { }

        private:
            const bool m_rotating; /**<  Is orientable. If true, game will try to apply a "snake effect". */
    };

    template<class Archive>
    void serialize(Archive &ar, Tile &tile, const unsigned int &version)
    {
        ar &boost::serialization::base_object<DatabaseItem>(tile);
        ar &tile.m_rotating;
    }
} /* End of namespace db */

#endif /* TILE_HPP */
