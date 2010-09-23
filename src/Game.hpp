#ifndef GAME_HPP
#define GAME_HPP

#include <list>
#include <vector>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Input.hpp>

namespace sf
{
    class RenderTarget;
}

class XSprite;
class Map;
class ArmyGeneral;

typedef std::pair<XSprite, std::string> p_renderingInfos;

class Game
{
    public:
        Game(sf::RenderTarget &target);
        ~Game();

        void initTestMap();

        void endTurn();
        void onMouseOver(const sf::Vector2i &mousePos);
        void listenInput(const sf::Input &Input);

        void renderGame();

        void startDrawingXSprite(XSprite *xsprite, const std::string &id);
        void stopDrawingXSprite(const std::string &id);

        void spawnUnit(const unsigned int &armyId, const std::string &type,
            const sf::Vector2i &pos);
        bool isUnitPresent(const sf::Vector2i &pos);
        ArmyGeneral *getArmy(const unsigned int &armyId);

        Map *getMapPtr() { return m_mapPtr; }

    private:
        sf::RenderTarget &target;
        Map *m_mapPtr;
        std::vector<ArmyGeneral*> m_armies;
        std::list<p_renderingInfos> m_renderingList;
};

#endif /* GAME_HPP */
