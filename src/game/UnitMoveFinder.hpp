#ifndef UNITMOVEFINDER_HPP
#define UNITMOVEFINDER_HPP

#include <string>
#include <list>
#include <SFML/System/Vector2.hpp>

class Map;
class GameEntity;
class Unit;

typedef std::list<sf::Vector2i> l_Vector2i;

/** \brief An algorithm that returns all tiles where a unit can move.
*/
class UnitMoveFinder
{
    public:
        static bool run(const Unit &unit, const Map &map,
            l_Vector2i &canMoveTo);

        static bool canUnitMoveTo(const std::string &unit,
            const std::string &tile);

    private:
        UnitMoveFinder() { }
        ~UnitMoveFinder() { }

        static bool addAdjacentTiles(const Map &map, const Unit &unit,
            sf::Vector2i &pos, l_Vector2i &alreadyAdded,
            const unsigned int &remainingCurrency);
        static bool tileAlreadyAdded(const l_Vector2i &in,
            const sf::Vector2i &tilePos);
};

#endif /* UNITMOVEFINDER_HPP */
