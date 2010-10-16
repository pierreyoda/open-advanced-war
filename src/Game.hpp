#ifndef GAME_HPP
#define GAME_HPP

#include <list>
#include <vector>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Input.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "db/Unit.hpp"
#include "game/XSprite.hpp"
#include "gui/EditorGui.hpp"

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
        void setTarget(sf::RenderTarget *ntarget)
        {
            if (ntarget != 0)
                target = ntarget;
        }

        void initTestMap();
        bool saveMap(const std::string &filename);
        bool loadMap(const std::string &filename);

        void endTurn();
        void onMouseOver(const sf::Vector2i &mousePos);
        void listenInput(const sf::Input &Input);
        void listenEvent(const sf::Event &Event);

        void renderGame(const float &frametime);

        void startDrawingXSprite(XSprite *xsprite, const std::string &id);
        void stopDrawingXSprite(const std::string &id);

        void spawnUnit(const unsigned int &armyId, const std::string &type,
            const sf::Vector2i &pos);
        bool isUnitPresent(const sf::Vector2i &pos);
        ArmyGeneral *getArmy(const unsigned int &armyId);

        int getGlobalAffector(const std::string &name);
        void setGlobalAffector(const std::string &name, const int &value);

        Map *getMapPtr() { return m_mapPtr; }

        void setEditorTile(const std::string &type) { m_tile = type; } // to delete (call lua instead)
        void setEditorBuilding(const std::string &type) { m_building = type; } // to delete (call lua instead)
        void setEditorUnit(const std::string &type) { m_unit = type; } // to delete (call lua instead)
        void setEditorFaction(const std::string &faction) { m_faction = faction; } // to delete (call lua instead)

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

        sf::RenderTarget *target;
        Map *m_mapPtr;
        std::vector<ArmyGeneral*> m_armies;
        std::list<p_renderingInfos> m_renderingList;
        std::list<db::IntCaracteristic> m_globalAffectors;
        bool m_inGame, m_inEditor;
        //InGameGui m_ingameGui;
        EditorGui m_editorGui;
        std::string m_tile, m_building, m_unit;
        std::string m_faction;
};

extern Game &gGame;

#endif /* GAME_HPP */
