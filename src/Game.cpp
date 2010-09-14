#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "db/DatabaseSerialization.hpp"
#include "tools/FilesPathHandler.hpp"
#include "Map.hpp"
#include "lua/LuaVirtualMachine.hpp"

Game::Game(sf::RenderTarget &target) : target(target), m_mapPtr(0)
{
    DatabaseSerialization::importFromXml("a");
    FilesPathHandler::scanDirectory("modules/Native/", gFph);
    LuaVM::getInstance().include(gFph("main.lua"));
    m_mapPtr = new Map();
}

Game::~Game()
{
    delete m_mapPtr;
}

void Game::onMouseOver(const sf::Vector2i &mousePos)
{
    sf::Vector2i mousePosTiles(GameEntity::pixelsToTiles(mousePos));
    if (m_mapPtr != 0)
        m_mapPtr->onMouseOver(mousePosTiles);
}

void Game::drawXSprite(XSprite &xsprite)
{
    xsprite.update();
    target.Draw(xsprite);
}
