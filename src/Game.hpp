#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/System.hpp>

class Map;

class Game
{
    public:
        Game();
        ~Game();

        void endTurn();
        void onMouseOver(const sf::Vector2i &mousePos);

        Map *getMapPtr() { return m_mapPtr; }

    private:
        Map *m_mapPtr;
};

#endif /* GAME_HPP */
