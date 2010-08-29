#include "Game.hpp"
#include "db/DatabaseSerialization.hpp"
#include "tools/FilesPathHandler.hpp"
#include "Map.hpp"
#include "lua/LuaVirtualMachine.hpp"

Game::Game() : m_mapPtr(0)
{
    DatabaseSerialization::importFromXml("a");
    FilesPathHandler::scanDirectory("modules/Native/", gFph);
    LuaVM::getInstance().include(gFph("main.lua"));
}

Game::~Game()
{
    delete m_mapPtr;
}
