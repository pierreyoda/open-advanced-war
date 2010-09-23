#ifndef GAME_HPP
#define GAME_HPP

#include <list>
#include <vector>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Input.hpp>
#include "db/Unit.hpp"
#include "game/XSprite.hpp"

namespace sf
{
    class RenderTarget;
}

class Map;
class ArmyGeneral;

typedef std::pair<XSprite, std::string> p_renderingInfos;

class Game
{
    friend class boost::serialization::access;

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

        int getGlobalAffector(const std::string &name);
        void setGlobalAffector(const std::string &name, const int &value);

        Map *getMapPtr() { return m_mapPtr; }

    private:
        Game() { }

        template <class Archive>
        void serialize(Archive &ar, const unsigned int &version)
        {
            /*ar &boost::serialization::make_nvp("map",
                (*m_mapPtr));*/
            ar &BOOST_SERIALIZATION_NVP(m_armies);
            //ar &BOOST_SERIALIZATION_NVP(m_renderingList);
            ar &m_globalAffectors;
        }

        sf::RenderTarget *target;
        Map *m_mapPtr;
        std::vector<ArmyGeneral*> m_armies;
        std::list<p_renderingInfos> m_renderingList;
        std::list<db::IntCaracteristic> m_globalAffectors;
};

#endif /* GAME_HPP */
