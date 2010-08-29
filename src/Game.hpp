#ifndef GAME_HPP
#define GAME_HPP

class Map;

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
