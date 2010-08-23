#ifndef GAME_HPP
#define GAME_HPP

#include "Map.hpp"

class Game
{
    public:
        Game();
        ~Game();

        void endTurn();

        Map *getMapPtr() { return m_mapPtr; }

    private:
        Map *m_mapPtr;
};

#endif /* GAME_HPP */
