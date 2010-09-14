#ifndef GAME_HPP
#define GAME_HPP

#include <vector>

namespace sf
{
    class RenderTarget;
}

class XSprite;
class Map;
class ArmyGeneral;

class Game
{
    public:
        Game(sf::RenderTarget &target);
        ~Game();

        void endTurn();
        void onMouseOver(const sf::Vector2i &mousePos);

        void drawXSprite(XSprite &xsprite);

        Map *getMapPtr() { return m_mapPtr; }

    private:
        sf::RenderTarget &target;
        Map *m_mapPtr;
        std::vector<ArmyGeneral*> m_armies;
};

#endif /* GAME_HPP */
