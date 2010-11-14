#ifndef GAME_HPP
#define GAME_HPP

#include <luabind/luabind.hpp>
#include <list>
#include <vector>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Input.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "db/Unit.hpp"
#include "game/XSprite.hpp"
#include "gui/GuiSpace.hpp"

namespace sf
{
    class RenderTarget;
}

class Map;
class ArmyGeneral;

typedef std::pair<XSprite, std::string> p_renderingInfos;

class Game : public Singleton<Game>
{
    friend class Singleton<Game>;
    friend class boost::serialization::access;

    public:
        void setApp(sf::RenderWindow *app)
        {
            if (app != 0)
                App = app;
        }

        void initTestMap();
        void newMap();
        bool saveMap(const std::string &filename);
        bool loadMap(const std::string &filename);

        void endTurn();
        void onMouseOver(const sf::Vector2i &mousePos);
        void listenInput(const sf::Input &Input);
        void listenEvent(const sf::Event &Event);

        void renderGame(const float &frametime);

        void startDrawingXSprite(XSprite *xsprite, const std::string &id);
        void stopDrawingXSprite(const std::string &id);

        void addGui(const std::string &name) { addGui(name, true); }
        void addGui(const std::string &name, const bool &setAsCurrent);
        void setCurrentGui(const std::string &name);

        void addArmy(const unsigned int &id, const std::string &name);
        void spawnUnit(const unsigned int &armyId, const std::string &type,
            const sf::Vector2i &pos);
        bool isUnitPresent(const sf::Vector2i &pos);
        ArmyGeneral *getArmy(const unsigned int &armyId);
        unsigned int nbOfArmies() const;

        int getGlobalAffector(const std::string &name);
        void setGlobalAffector(const std::string &name, const int &value);

        Map *getMapPtr() { return m_mapPtr; }

        int getChoiceFromTable(const luabind::object &table,
            const unsigned int &size, sf::FloatRect &rect);

        /*bool isInGame() const { return m_inGame; }
        bool isInEditor() const { return m_inEditor; }*/

        // internal (ugly)
        void unitDeleted(const sf::Vector2i &pos) { m_unitDeleted = true; m_unitDeletedPos = pos; }

    private:
        Game();
        ~Game();

        template <class Archive>
        void serialize(Archive &ar, const unsigned int &version)
        {
            /*ar &boost::serialization::make_nvp("map",
                (*m_mapPtr));*/
            ar &BOOST_SERIALIZATION_NVP(m_armies);
            //ar &BOOST_SERIALIZATION_NVP(m_renderingList);
            ar &m_globalAffectors;
        }

        sf::RenderWindow *App;
        Map *m_mapPtr;
        std::vector<ArmyGeneral*> m_armies;
        std::list<p_renderingInfos> m_renderingList;
        std::list<db::IntCaracteristic> m_globalAffectors;
        bool m_unitDeleted;
        sf::Vector2i m_unitDeletedPos;
        std::map<std::string, GuiSpace*> m_GUIs;
        std::string m_currentGui;
};

extern Game &gGame;

#endif /* GAME_HPP */
