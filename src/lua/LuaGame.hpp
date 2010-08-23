#ifndef LUAGAME_HPP
#define LUAGAME_HPP

/** \file LuaGame.hpp
* \brief Binds game to Lua.
*/

#include "../Map.hpp"

void exportGame(lua_State *lua)
{
    using namespace luabind;
    module(lua)
    [
        class_<Map>("Map")
            .def("getTile", (std::string(Map::*)(const unsigned int&,
                const unsigned int&)const)&Map::getTile)
            .def("setTile", (void(Map::*)(const unsigned int&,
                const unsigned int&, const std::string &))&Map::setTile)
    ];
}

#endif /* LUAGAME_HPP */