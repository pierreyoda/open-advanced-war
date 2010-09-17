#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "db/DatabaseSerialization.hpp"
#include "lua/LuaVirtualMachine.hpp"
#include "tools/FilesPathHandler.hpp"
#include "Map.hpp"
#include "game/ArmyGeneral.hpp"

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
}

void Game::onMouseOver(const sf::Vector2i &mousePos)
{
    sf::Vector2i mousePosTiles(GameEntity::pixelsToTiles(mousePos));
    if (m_mapPtr != 0)
        m_mapPtr->onMouseOver(mousePosTiles);
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
