#include "Game.hpp"

Game::Game() : m_mapPtr(0)
{

}

Game::~Game()
{
    delete m_mapPtr;
}
