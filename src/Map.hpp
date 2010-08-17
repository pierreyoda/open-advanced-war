#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include "game/Tile.hpp"

class RenderTarget;

class Map
{
    public:
        Map();
        ~Map();

        void renderTo(sf::RenderTarget *target);

    private:
        std::vector< std::vector<Tile> > m_tiles;
};

#endif /* MAP_HPP */
