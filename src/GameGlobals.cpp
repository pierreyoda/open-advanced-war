#include "GameGlobals.hpp"

GameGlobals *gg = &GameGlobals::getInstance();

GameGlobals::GameGlobals() : case_w(20), case_h(20)
{

}

GameGlobals::~GameGlobals()
{

}
