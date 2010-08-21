#ifndef GAMEGLOBALS_HPP
#define GAMEGLOBALS_HPP

#include "tools/Singleton.hpp"

/** \brief Stores game global variables.
*/
struct GameGlobals : public Singleton<GameGlobals>
{
    friend class Singleton<GameGlobals>;

    const unsigned int case_w, case_h;

    private:
        GameGlobals();
        GameGlobals(const GameGlobals &other) : case_w(20), case_h(20) { };
        ~GameGlobals();
};

extern GameGlobals *gg;

#endif /* GAMEGLOBALS_HPP */
