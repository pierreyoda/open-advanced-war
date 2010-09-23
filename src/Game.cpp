#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "db/DatabaseSerialization.hpp"
#include "lua/LuaVirtualMachine.hpp"
#include "tools/FilesPathHandler.hpp"
#include "Map.hpp"
#include "game/ArmyGeneral.hpp"
#include "game/GameEntity.hpp"

using namespace std;

Game::Game(sf::RenderTarget &target) : target(target), m_mapPtr(0)
{
    DatabaseSerialization::importFromXml("a");
    FilesPathHandler::scanDirectory("modules/Native/", gFph);
    LuaVM::getInstance().include(gFph("main.lua"));
}

Game::~Game()
{
    delete m_mapPtr;
}

void Game::initTestMap()
{
    m_mapPtr = new Map(); // for test - loading default map
    m_armies.push_back(new ArmyGeneral(m_armies.size(), "US"));
}

void Game::onMouseOver(const sf::Vector2i &mousePos)
{
    const sf::Vector2i mousePosTiles(GameEntity::pixelsToTiles(mousePos));
    Unit *ptr = 0;
    static Unit *prevUnit = 0;
    for (unsigned int i = 0; i < m_armies.size(); i++)
        if (m_armies[i] != 0)
        {
            Unit *ptr2 = m_armies[i]->getUnitPtr(mousePosTiles);
            if (ptr2 == 0)
                continue;
            ptr = ptr2;
            break;
        }
    static bool luaError =false, luaError2 = false;
    if (!luaError && prevUnit != 0)
    {
        CALL_LUA_FUNCTION(LuaVM::getInstance().getLua(), void,
            "onMouseNoMoreOverGameEntity", luaError, (GameEntity*)prevUnit) // upcasting
        prevUnit = 0;
    }
    if (ptr != 0)
    {
        if (!luaError2)
            CALL_LUA_FUNCTION(LuaVM::getInstance().getLua(), void,
                "onMouseOverGameEntity", luaError2, (GameEntity*)ptr) // upcasting
        prevUnit = ptr;
    }
    if (m_mapPtr != 0)
        m_mapPtr->onMouseOver(mousePosTiles, (ptr != 0));
}

void Game::listenInput(const sf::Input &Input)
{
    const sf::Vector2i mousePosTiles(GameEntity::pixelsToTiles(
        Input.GetMouseX(), Input.GetMouseY()));
    if (Input.IsMouseButtonDown(sf::Mouse::Left) && m_mapPtr != 0)
        m_mapPtr->setTile(mousePosTiles, "Road");
}

void Game::spawnUnit(const unsigned int &armyId, const string &type,
    const sf::Vector2i &pos)
{
    if (m_mapPtr != 0 && !m_mapPtr->isInsideMap(pos))
        return;
    ArmyGeneral *ptr = getArmy(armyId);
    if (ptr == 0)
    {
        cerr << "Error : no army of identifier '" << armyId << "'.\n";
        return;
    }
    ptr->addUnit(pos, type);
}

bool Game::isUnitPresent(const sf::Vector2i &pos)
{
    for (unsigned int i = 0; i < m_armies.size(); i++)
        if (m_armies[i] != 0 && m_armies[i]->getUnitId(pos) != 0) // unit here
            return true;
    return false;
}

ArmyGeneral *Game::getArmy(const unsigned int &armyId)
{
    for (unsigned int i = 0; i < m_armies.size(); i++)
        if (m_armies[i] != 0 && m_armies[i]->id() == armyId)
            return m_armies[i];
    return 0;
}

void Game::renderGame()
{
    if (m_mapPtr != 0)
        m_mapPtr->renderTo(target);
    for (unsigned int i = 0; i < m_armies.size(); i++)
    {
        if (m_armies[i] != 0)
            drawArmy(target, *m_armies[i]);
    }
    for (list<p_renderingInfos>::iterator iter = m_renderingList.begin();
        iter != m_renderingList.end(); iter++)
    {
        /*XSprite *ptr = (*iter).first;
        if (ptr == 0)
            continue;*/
        iter->first.update();
        target.Draw(iter->first);
    }
}

void Game::startDrawingXSprite(XSprite *xsprite, const string &id)
{
    if (xsprite == 0)
        return;
    for (list<p_renderingInfos>::const_iterator iter = m_renderingList.begin();
        iter != m_renderingList.end(); iter++)
    {
        if (iter->second == id)
        {
            cout << "[Game] Warning : xsprite '" << id
                << "' is already in rendering list. Ignoring...\n";
            return;
        }
    }
    m_renderingList.push_back(p_renderingInfos(*xsprite, id));
}

void Game::stopDrawingXSprite(const string &id)
{
    for (list<p_renderingInfos>::iterator iter = m_renderingList.begin();
        iter != m_renderingList.end(); iter++)
    {
        if (iter->second == id)
        {
           m_renderingList.erase(iter);
           return;
        }
    }
}
