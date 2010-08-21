#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include "game/GameEntity.hpp"

namespace sf
{
    class RenderTarget;
    typedef Vector2<unsigned int> Vector2ui; // For convenience
}

class Map
{
    public:
        Map(const sf::Vector2ui &size = sf::Vector2ui(15, 15));
        ~Map();

        void renderTo(sf::RenderTarget &target);

        const std::string getTile(const sf::Vector2ui &pos) const;
        const std::string getTile(const unsigned int &x, const unsigned int &y) const;
        void setTile(const sf::Vector2ui &pos, const std::string &type);
        void setTile(const unsigned int &x, const unsigned int &y,
            const std::string &type);

    private:
        std::vector< std::vector<GameEntity*> > m_tiles;
};

#endif /* MAP_HPP */
