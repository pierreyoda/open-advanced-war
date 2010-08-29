#include "GameGlobals.hpp"

GameGlobals &gg = GameGlobals::getInstance();

GameGlobals::GameGlobals() : screen_w(800), screen_h(600), case_w(20), case_h(20)
{

}

GameGlobals::~GameGlobals()
{

}
