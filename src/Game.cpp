#include "Game.hpp"
#include "db/DatabaseSerialization.hpp"
#include "tools/FilesPathHandler.hpp"

Game::Game() : m_mapPtr(0)
{
    DatabaseSerialization::importFromXml("a");
    FilesPathHandler::scanDirectory("modules/Native/", gFph);
}

Game::~Game()
{
    delete m_mapPtr;
}
